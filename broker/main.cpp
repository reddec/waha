//
// Created by RedDec on 17.07.15.
//
#include <tclap/CmdLine.h>
#include "broker.h"

int main(int argc, const char **argv) {
    TCLAP::CmdLine cmd("ZMQ broker (Dealer/Router)", ' ', "1.0.0", true);
    TCLAP::ValueArg<std::string> common_broker_url("u", "url", "Broker url", false, "tcp://*:10000",
                                                   "zmq bind");
    TCLAP::SwitchArg common_is_verbose("v", "verbose", "Show much more output", false);
    cmd.add(common_broker_url);
    cmd.add(common_is_verbose);

    cmd.parse(argc, argv);
    start_broker(common_broker_url.getValue(), common_is_verbose.getValue());
}