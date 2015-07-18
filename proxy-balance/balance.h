/*!
 * \file balance.h
 * \brief ZMQ load balancing based on DEALER-to-DEALER pattern
 * \author RedDec <net.dev@mail.ru>
 *
 * This file contains functions for balance proxy for broker.
 * Main idea is use one DEALER to ROUTER connection with multiple workers DEALER to DEALER.
 * So,service have to change only broker url param to proxy backend. Load balancing is ZMQ defined
 */
#ifndef WAHA_BALANCE_H
#define WAHA_BALANCE_H

#include <zmqpp/zmqpp.hpp>
#include <string>

/// \brief Bind backend, connect to broker and start ZMQ_PROXY DEALER-to-DEALER
/// \param broker Frontend broker URL. Ex: tcp://127.0.0.1:10000
/// \param local Backend binding used for service connection. Ex: tcp://*:10001
/// \param name Service name which will be balanced
/// \return zmqpp::exception on internal ZMQ error
static void start_balance_proxy(const std::string &broker, const std::string &local, const std::string &name) {
    zmqpp::context_t context;
    zmqpp::socket rep(context, zmqpp::socket_type::dealer);
    zmqpp::socket backend(context, zmqpp::socket_type::dealer);
    rep.set(zmqpp::socket_option::identity, name);
    rep.connect(broker);
    backend.bind(local);
    zmq_proxy(rep, backend, nullptr);
}

#endif //WAHA_BALANCE_H
