/*!
 * \file broker.cpp
 * \brief ZMQ broker implementation based on ZMQ_ROUTER
 * \author RedDec <net.dev@mail.ru>
 *
 * This file contains implementations of functions for controlling broker.
 */

#include "broker.h"
#include "utils.h"
#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <cstdint>


void start_broker(const std::string &binding, bool verbose) {
    if (verbose)
        std::clog << "Bound on: " << binding << std::endl;
    zmqpp::context_t context;
    zmqpp::socket router(context, zmqpp::socket_type::router);
    router.bind(binding);
    zmqpp::poller poller;
    poller.add(router);
    while (poller.poll()) {

        if (poller.has_input(router)) {
            zmqpp::message msg, packet;
            router.receive(msg);
            if (verbose) {
                std::clog << "Received" << std::endl;
                dump_message(std::clog, msg);
                std::clog << ". . . . . . . . . . . . . . . . " << std::endl;
                std::clog << "Description: ";
            }
            /// Detecting message type:
            /// Request is incoming from REQ to ROUTER, so it has zero separated packet
            /// else response
            if (msg.parts() >= 2 && msg.size(1) == 0) { //This is request from REQ
                if (verbose) {
                    std::clog << "request from ";
                    dump_str_or_hex(std::clog, msg.get(0));
                    std::clog << " to ";
                    dump_str_or_hex(std::clog, msg.get(2));
                    std::clog << std::endl;
                }
                /// Prepare request message to service:
                packet << msg.get(2); /// ServiceName - removed and restored by ROUTER to DEALER flow
                packet << msg.get(0); /// RequestID - simply REQ connection id. Must be first field on response
                /// ...Data...
                for (size_t part = 3; part < msg.parts(); ++part)
                    packet << msg.get(part);

            } else { //This is response
                if (verbose) {
                    std::clog << "response from ";
                    dump_str_or_hex(std::clog, msg.get(0));
                    std::clog << " to ";
                    dump_str_or_hex(std::clog, msg.get(1));
                    std::clog << std::endl;
                }
                /// Prepare response message - just remove first field ServiceName
                for (size_t part = 1; part < msg.parts(); ++part) {
                    packet << msg.get(part);
                    if (part == 1)
                        packet << "";
                }
            }
            if (verbose) {
                std::clog << ". . . . . . . . . . . . . . . . " << std::endl;
                std::clog << "Sent" << std::endl;
                dump_message(std::clog, packet);
                std::clog << "-------------------------------" << std::endl;
            }
            router.send(packet);

        }

    }
}