/*!
 * \file broker.h
 * \brief ZMQ broker implementation based on ZMQ_ROUTER
 * \author RedDec <net.dev@mail.ru>
 *
 * This file contains functions headers for controlling broker.
 * Broker is a just ZMQ_ROUTER.
 *
 * - Do request:
 *      Use REQ socket with first field of message as target service name
 *      others field may content anything else and will be copied to service
 *
 * - Be a service:
 *      Use DEALER socket with identity as service name.
 *      Each incoming requests have first field as RequestID which must be
 *      first field in response message.
 *      Important! Services can't use same identity. Use proxy-balance for multiple instances
 *
 * Future version may have one-way communication: request without reply
 * */
#ifndef WAHA_BROKER_H
#define WAHA_BROKER_H

#include <string>

/// \brief Bind and start broker on ZMQ Router socket.
///
/// \param binding ZMQ binding url. Ex. tcp://*:10000
/// \param verbose Show messages on each request-reply
/// \throw zmqpp::exception on internal zmq error
void start_broker(const std::string &binding, bool verbose);

#endif //WAHA_BROKER_H
