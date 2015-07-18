/*!
 * \file plain.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief  Read all stdin as single field
 *
 * This file contains implementation of #Plain reader
 * */
#include <sstream>
#include <iostream>
#include "plain.h"

zmqpp::message reader::Plain::read() {
    zmqpp::message msg;
    msg << target;
    std::vector<char> buffer = read_input();
    msg.add(buffer.data(), buffer.size());
    return msg;
}

std::vector<char> reader::Plain::read_input() {
    /// \todo Improve speed
    std::vector<char> buffer;
    while (!std::cin.eof()) {
        ulong c = std::cin.get();
        if (!std::cin.eof()) {
            buffer.push_back((char) c);
        } else
            break;
    }
    return buffer;
}

std::string reader::Plain::read_input_str() {
    auto buf = read_input();
    return std::string(buf.begin(), buf.end());
}
