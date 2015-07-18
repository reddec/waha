#include <tclap/CmdLine.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include "cli.h"
#include "readers/args.h"
#include "readers/plain.h"
#include "readers/json.h"
#include "readers/delimited.h"
#include "writers/empty.h"
#include "writers/plain.h"
#include "writers/json.h"
#include "writers/delimited.h"

int main(int argc, const char **argv) {
    TCLAP::CmdLine cmd("ZMQ broker console client interface", ' ', "1.0.0", true);
    TCLAP::ValueArg<std::string> common_broker_url("u", "url", "Broker url", false, "tcp://127.0.0.1:10000",
                                                   "zmq bind");
    std::vector<std::string> cli_format_lst = {"plain", "base64", "hex"};
    TCLAP::ValuesConstraint<std::string> cli_format_const(cli_format_lst);

    std::vector<std::string> cli_input_lst = {"args", "plain", "delimited", "json"};
    TCLAP::ValuesConstraint<std::string> cli_input_const(cli_input_lst);
    TCLAP::ValueArg<std::string> cli_input("i", "input", "Input mode in CLI mode", false, "args", &cli_input_const);
    TCLAP::ValueArg<char> cli_input_sep("", "in-sep", "Input separator", false, '\n', "char");
    TCLAP::ValueArg<std::string> cli_input_decoder("d", "decoder", "Input decoder for 'delimited' input", false,
                                                   "plain", &cli_format_const);
    cmd.add(cli_input);
    cmd.add(cli_input_sep);
    cmd.add(cli_input_decoder);

    std::vector<std::string> cli_output_lst = {"empty", "plain", "delimited", "json"};
    TCLAP::ValuesConstraint<std::string> cli_output_const(cli_output_lst);
    TCLAP::ValueArg<std::string> cli_output("o", "output", "Output mode in CLI mode", false, "delimited",
                                            &cli_output_const);
    TCLAP::ValueArg<char> cli_output_sep("", "out-sep", "Output separator", false, '\n', "char");
    TCLAP::ValueArg<std::string> cli_output_encoder("e", "encoder", "Output encoder for 'delimited' output", false,
                                                    "plain", &cli_format_const);
    TCLAP::SwitchArg cli_output_json_pretty("p", "pretty", "Pretty JSON output for 'json'", false);
    cmd.add(cli_output);
    cmd.add(cli_output_sep);
    cmd.add(cli_output_encoder);
    cmd.add(cli_output_json_pretty);


    TCLAP::ValueArg<std::string> common_name("n", "name", "Remote service name", true, "unknown", "name");
    TCLAP::SwitchArg common_is_verbose("v", "verbose", "Show much more output", false);
    TCLAP::ValueArg<int> common_timeout("t", "timeout", "Request timeout. -1 - infinity", false, 5000, "ms");

    /// [Common arguments]

    TCLAP::UnlabeledMultiArg<std::string> client_args("args", "Args in request message for for 'args' input", false,
                                                      "string");


    cmd.add(common_broker_url);
    cmd.add(common_is_verbose);
    cmd.add(common_name);
    cmd.add(common_timeout);
    cmd.add(client_args);
    cmd.parse(argc, argv);

    CLI client;
    client.setVerbose(common_is_verbose.getValue());
    if (cli_input.getValue() == "args") {
        client.setInput(std::make_shared<reader::Args>(client_args.getValue()));
    } else if (cli_input.getValue() == "plain") {
        client.setInput(std::make_shared<reader::Plain>());
    } else if (cli_input.getValue() == "json") {
        client.setInput(std::make_shared<reader::Json>());
    } else {
        client.setInput(std::make_shared<reader::Delimited>(cli_input_sep.getValue(),
                                                            reader::Delimited::decoder_by_name(
                                                                    cli_input_decoder.getValue())));
    }
    client.getInput()->setVerbose(common_is_verbose.getValue());
    client.getInput()->setTarget(common_name.getValue());

    if (cli_output.getValue() == "empty") {
        client.setOutput(std::make_shared<writer::Empty>());
    } else if (cli_output.getValue() == "plain") {
        client.setOutput(std::make_shared<writer::Plain>());
    } else if (cli_output.getValue() == "json") {
        client.setOutput(std::make_shared<writer::Json>(cli_output_json_pretty.getValue()));
    } else {
        client.setOutput(std::make_shared<writer::Delimited>(
                cli_output_sep.getValue(),
                writer::Delimited::encoder_by_name(cli_output_encoder.getValue())
        ));
    }
    client.setVerbose(common_is_verbose.getValue());
    return client.start(common_broker_url.getValue(), common_timeout.getValue());
}