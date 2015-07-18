/*!
 * \file json.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief JSON input reader
 *
 * This file contains implementation of #Json reader
 * */
#include "json.h"
#include "plain.h"
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <sstream>

zmqpp::message reader::Json::read() {
    zmqpp::message msg;
    msg << target;
    ::Json::Value value;
    ::Json::Reader reader;
    if (!reader.parse(Plain::read_input_str(), value)) {
        throw std::runtime_error(reader.getFormatedErrorMessages());
    }
    if (!value.isArray()) {
        throw std::runtime_error("Must be array");
    }
    for (size_t i = 0; i < value.size(); ++i) {
        auto val = value.get(i, "");
        std::string param = (val.isString() ? val.asString() : val.toStyledString());
        if (!val.isString()) {
            if (verbose) {
                std::cerr << "Item #" << i << " is not string!" << std::endl;
            }
            param = trim_newline(param);
        }
        msg << param;
    }
    return msg;
}

std::string reader::Json::trim_newline(const std::string &t) {
    if (t[t.size() - 1] == '\n')return t.substr(0, t.size() - 1);
    return t;
}
