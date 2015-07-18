/*!
 * \file plain.cpp
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief  Plain message writer to stdout
 *
 * This file contains implementation of #Plain writer
 * */

#include <iostream>
#include "plain.h"

void writer::Plain::write(zmqpp::message &msg) {
    for (size_t i = 0; i < msg.parts(); ++i) {
        std::cout.write((char *) msg.raw_data(i), msg.size(i));
    }
    std::cout.flush();
}
