/*!
 * \file delimited.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief This file contains implementation of #Delimited reader
 *
 *
 * */
#include <Poco/HexBinaryDecoder.h>
#include <Poco/Base64Decoder.h>
#include <iostream>
#include <sstream>
#include "delimited.h"

zmqpp::message reader::Delimited::read() {
    zmqpp::message msg;
    msg << target;
    while (!std::cin.eof()) {
        std::string line;
        /// Read part till separator
        std::getline(std::cin, line, delimiter);
        if (line.empty() && std::cin.eof())break;
        /// Decode part and add field to message
        add_decoded(line, msg);
    }
    return msg;
}

void reader::Delimited::add_decoded(const std::string &line, zmqpp::message &msg) {
    switch (decoder) {
        case Decoder::BASE64: {
            std::istringstream istr(line);
            std::ostringstream ostr;
            Poco::Base64Decoder b64in(istr);
            copy(std::istreambuf_iterator<char>(b64in),
                 std::istreambuf_iterator<char>(),
                 std::ostreambuf_iterator<char>(ostr));
            msg << ostr.str();
            break;
        }
        case Decoder::HEX: {
            std::istringstream istr(line);
            std::ostringstream ostr;
            Poco::HexBinaryDecoder hexin(istr);
            copy(std::istreambuf_iterator<char>(hexin),
                 std::istreambuf_iterator<char>(),
                 std::ostreambuf_iterator<char>(ostr));
            msg << ostr.str();
            break;
        }
        case Decoder::Plain:
        default:
            msg << line;
    }
}

char reader::Delimited::getDelimiter() const {
    return delimiter;
}

void reader::Delimited::setDelimiter(char delimiter) {
    Delimited::delimiter = delimiter;
}

reader::Delimited::Decoder reader::Delimited::getDecoder() const {
    return decoder;
}

void reader::Delimited::setDecoder(reader::Delimited::Decoder decoder) {
    Delimited::decoder = decoder;
}


