/*!
 * \file delimited.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief  Writer of message fields joined by separator
 *
 * This file contains implementation of #Delimited Writer
 * */
#include <iostream>
#include <sstream>
#include <Poco/Base64Encoder.h>
#include <Poco/HexBinaryEncoder.h>
#include "delimited.h"

void writer::Delimited::write(zmqpp::message &msg) {
    for (size_t i = 0; i < msg.parts(); ++i) {
        const char *data = (const char *) msg.raw_data(i);
        size_t size = msg.size(i);
        switch (encoder) {
            case BASE64: {
                std::istringstream istr(msg.get(i));
                Poco::Base64Encoder b64out(std::cout);
                copy(std::istreambuf_iterator<char>(istr),
                     std::istreambuf_iterator<char>(),
                     std::ostreambuf_iterator<char>(b64out));
                break;
            }
            case HEX: {
                std::istringstream istr(msg.get(i));
                Poco::HexBinaryEncoder hexout(std::cout);
                copy(std::istreambuf_iterator<char>(istr),
                     std::istreambuf_iterator<char>(),
                     std::ostreambuf_iterator<char>(hexout));
                break;
            }
            case Plain:
            default:
                std::cout.write(data, size);
        }
        if (i != msg.parts() - 1)
            /// Do not print delimiter if it is last part
            std::cout.put(delimiter);
        std::cout.flush();
    }
}

char writer::Delimited::getDelimiter() const {
    return delimiter;
}

void writer::Delimited::setDelimiter(char delimiter) {
    Delimited::delimiter = delimiter;
}

writer::Delimited::Encoder writer::Delimited::getEncoder() const {
    return encoder;
}

void writer::Delimited::setEncoder(writer::Delimited::Encoder encoder) {
    Delimited::encoder = encoder;
}

writer::Delimited::Encoder  writer::Delimited::encoder_by_name(const std::string &name) {
    if (name == "hex")return HEX;
    if (name == "base64" || name == "b64")return BASE64;
    return Plain;
}