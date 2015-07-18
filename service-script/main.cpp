#include <tclap/CmdLine.h>
#include <iostream>
#include <cstdint>
#include "process.h"

int main(int argc, const char **argv) {
    TCLAP::CmdLine cmd("Broker script service", ' ', "1.0.0", true);
    TCLAP::ValueArg<std::string> common_broker_url("u", "url", "Broker url", false, "tcp://127.0.0.1:10000",
                                                   "zmq bind");
    TCLAP::ValueArg<std::string> script_path("s", "script", "Script path for 'script' mode", false, "/bin/echo",
                                             "script");

    TCLAP::SwitchArg script_ret_code("r", "ret-code", "Append script return code as last field in message", false);
    TCLAP::SwitchArg script_with_stderr("e", "stderr", "Append script stderr output", false);
    TCLAP::SwitchArg script_no_output("", "no-stdout", "Do not use stdout as message", false);
    TCLAP::ValueArg<std::string> common_name("n", "name", "Balancing service name", true, "unknown", "name");
    TCLAP::SwitchArg common_is_verbose("v", "verbose", "Show much more output", false);
    TCLAP::UnlabeledMultiArg<std::string> script_args("args", "Predefined args for script", false, "string");


    cmd.add(common_broker_url);
    cmd.add(common_is_verbose);
    cmd.add(common_name);
    cmd.add(script_ret_code);
    cmd.add(script_with_stderr);
    cmd.add(script_no_output);
    cmd.add(script_path);
    cmd.add(script_args);
    cmd.parse(argc, argv);

    start_script(common_broker_url.getValue(),
                 common_name.getValue(),
                 script_path.getValue(),
                 script_args.getValue(),
                 script_ret_code.getValue(),
                 script_with_stderr.getValue(),
                 script_no_output.getValue(),
                 common_is_verbose.getValue());
}