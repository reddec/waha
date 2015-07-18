/*!
 * \file cli.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Command line adapter
 *
 * This file contains headers of command line adapter functions
 *
 * CLI can be used as common full-featured utility for requesting
 * services with different input and output format. Returns non-zero code on error.
 *
 * Input:
 *  - plain mode with all input (stdin) data as single message field
 *  - args mode with application arguments as message fields
 *  - json mode with input data as JSON array used as message fields
 *  - delimited complex mode with input data delimtited by separator used as message fields.
 *    each part of input can be decoded (ex: for binary data) as
 *      - plain - as-is
 *      - hex - binhex encoding
 *      - base64 - base64 encoding
 *
 * Output:
 *  - plain mode with all message fields concatenated as-is
 *  - empty mode without any output
 *  - json mode with message fields as JSON array
 *  - delimited mode with message fields are joined by separator with each field decoding:
 *      - plain - as-is
 *      - hex - binhex deoding
 *      - base64 - base64 decoding
 *
 * */

#ifndef WAHA_CLI_H
#define WAHA_CLI_H

#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>
#include "readers/reader.h"
#include "writers/writer.h"
#include "utils.h"

/*!
 * \brief Comman line interface
 *
 * Setups output and input.
 * Sends request and waits reply
 */
class CLI {
private:
    std::shared_ptr<Reader> input;
    std::shared_ptr<writer::Writer> output;
    bool verbose = false;

public:

    const std::shared_ptr<Reader> &getInput() const {
        return input;
    }

    void setInput(const std::shared_ptr<Reader> &input) {
        CLI::input = input;
    }

    const std::shared_ptr<writer::Writer> &getOutput() const {
        return output;
    }

    void setOutput(const std::shared_ptr<writer::Writer> &output) {
        CLI::output = output;
    }

    bool isVerbose() const {
        return verbose;
    }

    void setVerbose(bool verbose) {
        CLI::verbose = verbose;
    }

    /*!
     * \brief Connect to broker, read input message, sends request, wait reply and render outpur
     * \param front_broker_router ZMQ Broker url
     * \param timeout Request timeout
     * \return non-zero code on error
     * \throw zmqpp::exception on internal ZMQ error
     */
    int start(const std::string &frontend_broker_router, int timeout) {
        if (!input) {
            if (verbose)std::clog << "Input reader not set" << std::endl;
            return 1;
        }
        if (!output) {
            if (verbose)std::clog << "Output writer not set" << std::endl;
            return 2;
        }
        RequestClient client;
        if (verbose)
            std::clog << "Connecting..." << std::endl;
        client.connect(frontend_broker_router);
        zmqpp::message msg = input->read();
        zmqpp::message out;
        if (verbose)
            std::clog << "Requesting with timeout " << timeout << " ms" << std::endl;
        if (!client.send_request(msg, out, timeout)) {
            if (verbose)
                std::clog << "Request timeout" << std::endl;
            return 3;
        }
        if (verbose)
            std::clog << "Encoding..." << std::endl;
        output->write(out);
        return 0;
    }
};

#endif //WAHA_CLI_H
