/*!
 * \file args.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Use arguments as message fields
 *
 * This file contains implementation of #Args reader
 * */
#include "args.h"

zmqpp::message reader::Args::read() {
    zmqpp::message msg;
    msg << target;
    for (auto &arg:args) {
        msg << arg;
    }
    return msg;
}

const std::vector<std::string> &reader::Args::getArgs() const {
    return args;
}

void reader::Args::setArgs(const std::vector<std::string> &args) {
    Args::args = args;
}