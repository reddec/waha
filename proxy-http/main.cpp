#include <tclap/CmdLine.h>
#include <iostream>
#include <cstdint>
#include "http.h"

int main(int argc, const char **argv) {
    TCLAP::CmdLine cmd("Broker HTTP proxy (REST like) module", ' ', "1.0.0", true);
    TCLAP::ValueArg<std::string> common_broker_url("u", "url", "Broker url", false, "tcp://127.0.0.1:10000",
                                                   "zmq bind");
    TCLAP::ValueArg<int> http_port("p", "port", "HTTP binding port", false, 9001, "int");
    TCLAP::ValueArg<int> http_threads("", "threads", "HTTP max threads", false, 8, "int");
    TCLAP::ValueArg<int> common_timeout("t", "timeout",
                                        "Request timeout  without `timeout` param. -1 - infinity",
                                        false, 5000, "ms");
    TCLAP::SwitchArg common_is_verbose("v", "verbose", "Show much more output", false);
    cmd.add(common_broker_url);
    cmd.add(common_is_verbose);
    cmd.add(http_port);
    cmd.add(http_threads);
    cmd.add(common_timeout);
    cmd.parse(argc, argv);
    start_http_proxy(http_port.getValue(), common_timeout.getValue(), http_threads.getValue(),
                     common_broker_url.getValue(), common_is_verbose.getValue());
}