/*!
 * \file http.h
 * \brief HTTP REST-like adapter for ZMQ broker
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 *
 * This file contains functions headers for HTTP adapter for ZMQ broker.
 * Path (means all text after '/' without query) uses as a service name
 * HTTP adapter is using fields in GET (url params):
 * - 'args' is required param which must contains JSON array of message fields
 * - 'type' is optional param (default 'json') which determinate response type: plain, json
 * - 'callback' or 'jsonp' is optional param which is using with JSON type for JSON-P request
 * - 'timeout' is optional param of service request timeout
 *
 * 'args' also can be a JSON array in POST body
 * \todo: refactor includes
 * */
#ifndef WAHA_HTTP_H
#define WAHA_HTTP_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/ServerSocket.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <Poco/URI.h>
#include <zmqpp/zmqpp.hpp>
#include <stdint.h>
#include <unordered_set>
#include "utils.h"
#include <tclap/CmdLine.h>

/*!
 * \brief HTTP request processor
 *
 * This class sends request to service and waits reply in one thread.
 */
class RESTHandler : public Poco::Net::HTTPRequestHandler {
    /// Because of thread pool we have to different instance of broker client
    static thread_local RequestClient client;
    /// Because of thread pool we have to different instance of JSON parser
    static thread_local Json::Reader reader;
    /// Because of thread pool we have to different instance of JSON printer
    static thread_local Json::FastWriter writer;
    /// Black list of requests path (ex: /favicon.ico)
    static std::unordered_set<std::string> black_list;

    int timeout = -1;
    const std::string &broker;
    bool verbose = false;

    bool build_message(Poco::Net::HTTPServerRequest &req, Poco::Net::HTMLForm &form, Poco::URI &url,
                       zmqpp::message &msg);

    static void error_parse(Poco::Net::HTTPServerResponse &response);

    static void error_timeout(Poco::Net::HTTPServerResponse &response);

    static void error_black_list(Poco::Net::HTTPServerResponse &response);

public:

    /// \param timeout default request timeout in ms
    /// \param broker ZMQ broker url
    /// \param verbose Print each request - reply
    RESTHandler(int timeout, const std::string &broker, bool verbose);

    /// \brief Handle HTTP request, parse arguments, send request, wait reply and render in single thread
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

/*!
 * \brief HTTP request processor factory
 *
 * This class creates new handler for each request
 */
class RESTHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
    int timeout = -1;
    std::string broker;
    bool verbose;
public:

    RESTHandlerFactory(int timeout, const std::string &broker, bool verbose);

    virtual Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request);
};

/// \brief Start http proxy and pause
/// \param port HTTP server binding port
/// \param timeout Default timeout in ms if no 'timeout' param in request
/// \param threads Thread pool size
/// \param url ZMQ broker url
/// \verbose Print each request and reply
/// \throw zmqpp::exception on internal ZMQ error
void start_http_proxy(int port,
                      int timeout,
                      int threads,
                      const std::string &url,
                      bool verbose);

#endif //WAHA_HTTP_H
