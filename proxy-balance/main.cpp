#include <tclap/CmdLine.h>
#include <iostream>
#include <cstdint>
#include "balance.h"

int main(int argc, const char **argv) {
    TCLAP::CmdLine cmd("Broker balance module", ' ', "1.0.0", true);
    TCLAP::ValueArg<std::string> common_broker_url("u", "url", "Broker url", false, "tcp://127.0.0.10000",
                                                   "zmq bind");
    TCLAP::ValueArg<std::string> proxy_backend("b", "backend", "Backend binding for proxy", false,
                                               "tcp://127.0.0.1:10001",
                                               "zmq bind");
    TCLAP::ValueArg<std::string> common_name("n", "name", "Balancing service name", true, "unknown", "name");
    TCLAP::SwitchArg common_is_verbose("v", "verbose", "Show much more output", false);
    cmd.add(common_broker_url);
    cmd.add(common_name);
    cmd.add(common_is_verbose);
    cmd.add(proxy_backend);
    cmd.parse(argc, argv);
    start_balance_proxy(common_broker_url.getValue(), proxy_backend.getValue(), common_name.getValue());
}