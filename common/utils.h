/*!
 * \file utils.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Collection of usefull utils
 *
 * This file contains header and partial implementation of common functions
 * */

#ifndef WAHA_UTILS_H
#define WAHA_UTILS_H

#include <zmqpp/zmqpp.hpp>
#include <zmqpp/message.hpp>
#include <iostream>
#include <istream>
#include <iomanip>

/// \brief Write to stream string or HEX if exists non-printable symbols
/// \param out std::ostream like stream
/// \param s text to check and print
template<class stream>
static void dump_str_or_hex(stream &out, const std::string &s) {
    bool has_unpritable = false;
    for (size_t i = 0; i < s.size() && !has_unpritable; ++i)
        has_unpritable = !std::isprint(s[i]);
    if (has_unpritable) {
        out << std::hex << std::uppercase << std::setfill('0');
        out << '[';
        for (size_t i = 0; i < s.size(); ++i) {
            if (i != 0)
                out << ' ';
            out << std::setw(2) << ((uint) ((unsigned char) s[i]));

        }
        out << ']';
        out << std::dec << std::nouppercase << std::setfill(' ');
    } else
        out << s;
}

/// \brief Write to stream message body
/// \param out std::ostream like stream
/// \param msg ZMQ message
template<class stream>
static void dump_message(stream &out, zmqpp::message &msg) {
    for (size_t i = 0; i < msg.parts(); ++i) {
        out << i << '\t';
        dump_str_or_hex(out, msg.get(i));
        out << std::endl;
    }
}

/*!
 * \brief Simple request reply wrapper over ZMQ context and socket
 * Thread safe
 */
struct RequestClient {
    static thread_local zmqpp::context context;
    zmqpp::socket request;
    zmqpp::poller poller;


    RequestClient();

    bool send_request(zmqpp::message &in, zmqpp::message &out, int timeout = -1);

    void connect(const std::string &url_);

    static size_t context_id();

private:
    std::string url;

    void reinitialize();
};

#endif //WAHA_UTILS_H
