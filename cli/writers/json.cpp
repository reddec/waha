/*!
 * \file json.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Write JSON to output
 *
 * This file contains implementation of #Json writer
 * */

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include "json.h"

void writer::Json::write(zmqpp::message &msg) {
    ::Json::Value value(::Json::ValueType::arrayValue);
    for (size_t i = 0; i < msg.parts(); ++i) {
        value.append(msg.get(i));
    }
    if (pretty) {
        ::Json::StyledWriter w;
        std::cout << w.write(value) << std::flush;
    } else {
        ::Json::FastWriter w;
        std::cout << w.write(value) << std::flush;
    }
}

bool writer::Json::isPretty() const {
    return pretty;
}

void writer::Json::setPretty(bool pretty) {
    Json::pretty = pretty;
}