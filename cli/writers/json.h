/*!
 * \file json.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Write JSON to output
 *
 * This file contains headers of #Json writer
 * */

#include "writer.h"

#ifndef WAHA_WRITER_JSON_H
#define WAHA_WRITER_JSON_H

namespace writer {
    /*!
     * \brief Write JSON to output
     *
     *  Output can be single-line or with indents (pretty = true)
     */
    class Json : public Writer {
        bool pretty = false;

    public:


        Json() { }

        Json(bool pretty) : pretty(pretty) { }

        bool isPretty() const;

        void setPretty(bool pretty);


        virtual void write(zmqpp::message &msg);
    };
}

#endif //WAHA_JSON_H
