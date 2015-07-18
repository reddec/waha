/*!
 * \file http.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Implementation of HTTP proxy
 *
 * This file contains implementation of HTTP adapter
 * */

#include <iostream>
#include <istream>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include "http.h"

thread_local Json::Reader RESTHandler::reader;
thread_local Json::FastWriter RESTHandler::writer;
thread_local RequestClient RESTHandler::client;
std::unordered_set<std::string> RESTHandler::black_list = {"/favicon.ico"};


RESTHandler::RESTHandler(int timeout, const std::string &broker, bool verbose) : HTTPRequestHandler(),

                                                                                 timeout(timeout), broker(broker),
                                                                                 verbose(verbose) { }

bool RESTHandler::build_message(Poco::Net::HTTPServerRequest &request, Poco::Net::HTMLForm &form, Poco::URI &url,
                                zmqpp::message &msg) {

    Json::Value root;
    bool ok = false;
    /// Find 'args' param in query or as POST body
    if (form.has("args")) {
        ok = reader.parse(form.get("args"), root);
    } else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) {
        ok = reader.parse(request.stream(), root);
    }
    if (!ok || !root.isArray()) {
        return false;
    }
    if (verbose) {
        std::clog << "0\t" << url.getPath().substr(1) << std::endl;
    }
    /// Get service name as path without leading slash
    msg << url.getPath().substr(1);
    for (size_t i = 0; i < root.size(); ++i) {
        auto val = root.get(i, "");
        if (!verbose)
            msg << (val.isString() ? root.get(i, "").asString() : val.toStyledString());
        else {
            std::string s = (val.isString() ? root.get(i, "").asString() : val.toStyledString());
            msg << s;
            std::clog << (i + 1) << '\t' << s << std::endl;
        }
    }
    return true;
}

void RESTHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                Poco::Net::HTTPServerResponse &response) {
    if (verbose) {
        std::clog << "HTTP request " << request.getURI() << std::endl;
        std::clog << "Context id: " << client.context_id() << std::endl;
    }
    zmqpp::message msg, reply;
    /// Connect to broker if not connected
    client.connect(broker);
    Poco::URI url(request.getURI());
    Poco::Net::HTMLForm form(request);
    /// Filter by black list
    if (black_list.find(url.getPath()) != black_list.end()) {
        return error_black_list(response);
    }
    if (!build_message(request, form, url, msg)) {
        return error_parse(response);
    }
    if (!client.send_request(msg, reply, (form.has("timeout") ? std::stoi(form.get("timeout")) : timeout))) {
        return error_timeout(response);
    }
    /// Render response
    response.setStatus(Poco::Net::HTTPServerResponse::HTTPStatus::HTTP_OK);
    if (form.get("type", "json") == "json") {
        /// JSON in single line (FastWriter)
        std::string jsonp_callback = form.get("jsonp", form.get("callback", ""));
        Json::Value packet(Json::ValueType::arrayValue);
        response.setContentType("application/json");
        std::ostream &out = response.send();
        if (!jsonp_callback.empty())
            out << jsonp_callback << "(";
        for (size_t part = 0; part < reply.parts(); ++part)
            packet.append(reply.get(part));
        auto txt = writer.write(packet);
        if (txt[txt.size() - 1] == '\n') // Cheat for EOL in serialization
            txt = txt.substr(0, txt.size() - 1);
        out << txt << (!jsonp_callback.empty() ? ")" : "") << std::flush;
    } else {
        /// Plain text wihtout delimiters
        response.setContentType("text/plain");
        std::ostream &out = response.send();
        for (size_t part = 0; part < reply.parts(); ++part)
            out.write((char *) reply.raw_data(part), reply.size(part));
        out.flush();
    }


}

RESTHandlerFactory::RESTHandlerFactory(int timeout, const std::string &broker, bool verbose)
        : HTTPRequestHandlerFactory(),
          timeout(timeout), broker(broker),
          verbose(verbose) { }

Poco::Net::HTTPRequestHandler *RESTHandlerFactory::createRequestHandler(
        const Poco::Net::HTTPServerRequest &) {
    return new RESTHandler(timeout, broker, verbose);
}

void start_http_proxy(int port, int timeout, int threads, const std::string &url, bool verbose) {
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams();
    params->setMaxQueued(50 * threads);
    params->setMaxThreads(threads);
    Poco::Net::ServerSocket sock(port);
    Poco::Net::HTTPServer server(new RESTHandlerFactory(timeout, url, verbose), sock, params);
    server.start();
    pause();
    server.stop();
}


void RESTHandler::error_parse(Poco::Net::HTTPServerResponse &response) {
    response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_BAD_REQUEST, "Parse failed");
    response.send() << "Invalid JSON arguments" << std::flush;
}

void RESTHandler::error_timeout(Poco::Net::HTTPServerResponse &response) {
    response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_REQUEST_TIMEOUT, "External system timeout");
    response.send() << "External system request timeout" << std::flush;
}

void RESTHandler::error_black_list(Poco::Net::HTTPServerResponse &response) {
    response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_NOT_FOUND, "Resource in blacklist");
    response.send() << "Requested resource is in black-list" << std::flush;
}

