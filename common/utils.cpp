/*!
 * \file utils.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief
 *
 * This file contains implementation of common functions
 * */
#include "utils.h"

thread_local zmqpp::context RequestClient::context;

RequestClient::RequestClient() : request(context, zmqpp::socket_type::req) { }

bool RequestClient::send_request(zmqpp::message &in, zmqpp::message &out, int timeout) {
    request.send(in);
    if (poller.poll(timeout)) {
        request.receive(out);
        return true;
    }
    reinitialize();
    return false;
}

void RequestClient::connect(const std::string &url_) {
    if (url.empty()) {
        request.connect(url_);
        url = url_;
        poller.add(request);
    } else if (!url_.empty() && url != url_) {
        url = url_;
        reinitialize();
    }

}

void RequestClient::reinitialize() {
    request.close();
    request = zmqpp::socket(context, zmqpp::socket_type::req);
    poller = zmqpp::poller();
    poller.add(request);
    request.connect(url);
}

size_t RequestClient::context_id() {
    return (size_t) ((void *) context);
}