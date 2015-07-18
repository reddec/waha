/*!
 * \file plain.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Read all stdin as single field
 *
 * This file contains headers of #Plain reader
 * */
#ifndef WAHA_PLAIN_H
#define WAHA_PLAIN_H

#include "reader.h"

namespace reader {
    /*!
     * \brief Read all stdin (till EOF) as single field
     *
     */
    class Plain : public Reader {

    public:

        virtual zmqpp::message read();

        static std::vector<char> read_input();

        static std::string read_input_str();
    };
}

#endif //WAHA_PLAIN_H
