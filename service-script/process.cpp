/*!
 * \file process.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Execute external program as service
 *
 *  Implementation of script-service
 * */

#include "process.h"
#include <Poco/Process.h>
#include <Poco/Pipe.h>
#include <Poco/PipeStream.h>
#include <zmqpp/zmqpp.hpp>
#include <iostream>

/// \brief read all data from pipe
/// \param pipe Bytes channel
/// \param chunk_size Size of read block
/// \return Collected data
std::vector<char> read_all(Poco::Pipe &pipe, size_t chunk_size = 8192) {
    std::vector<char> buf, chunk(chunk_size);
    ssize_t s = 1;
    while ((s = pipe.readBytes(chunk.data(), chunk_size)) > 0) {
        buf.insert(buf.end(), chunk.begin(), chunk.begin() + s);
    }
    return buf;
}

void start_script(const std::string &broker_url,
                  const std::string &name,
                  const std::string &script,
                  const std::vector<std::string> &predefined_args,
                  bool with_ret_code,
                  bool with_stderr,
                  bool disable_stdout,
                  bool verbose) {
    zmqpp::context_t context;
    zmqpp::socket rep(context, zmqpp::socket_type::dealer);
    rep.set(zmqpp::socket_option::identity, name);
    rep.connect(broker_url);
    if (verbose)
        std::cerr << "Started as script service " << name << " to " << broker_url << " for " << script << std::endl;
    for (; ;) {

        zmqpp::message msg;
        if (!rep.receive(msg))break;

        Poco::Process::Args args;
        /// Prepare arguments based on predefined
        args.insert(args.end(), predefined_args.begin(), predefined_args.end());
        /// Copy message fields as additional arguments
        for (size_t part = 1; part < msg.parts(); ++part) {
            if (msg.size(part) > 0)
                args.push_back(msg.get(part));
        }
        if (verbose) {
            std::clog << "Executing " << script << std::endl;
        }
        Poco::Pipe outPipe;
        if (verbose) {
            for (auto &arg:args) {
                std::clog << arg << std::endl;
            }
        }
        /// Start script
        auto proc = Poco::Process::launch(script, args, nullptr, (disable_stdout ? nullptr : &outPipe),
                                          (with_stderr ? &outPipe : nullptr));

        zmqpp::message reply;
        /// Set RequestID
        reply << msg.get(0);
        if (!disable_stdout) {
            /// Collect data if output required
            auto content = read_all(outPipe);
            if (verbose) {
                std::clog << "Content: ";
                std::clog.write(content.data(), content.size());
                std::clog << std::endl;
            }
            /// Set data
            reply.add(content.data(), content.size());
        }
        if (with_ret_code) {
            /// Append return code if required
            reply << std::to_string(proc.wait());
        }
        rep.send(reply);
    }
}