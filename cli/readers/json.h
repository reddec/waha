/*!
 * \file json.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief JSON input reader
 *
 *  This file contains headers of #Json reader
 * */
#ifndef WAHA_JSON_H
#define WAHA_JSON_H

#include "reader.h"

namespace reader {
    /*!
     * \brief JSON input reader
     *
     * Allows only JSON array.
     * Warning in verbose mode will be printed if array item is not string
     */
    class Json : public Reader {

    public:
        virtual zmqpp::message read();

        /// \brief Remove trailing \n if exist
        static std::string trim_newline(const std::string &t);
    };
}

#endif //WAHA_JSON_H
