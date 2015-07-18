/*!
 * \file plain.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Plain message writer to stdout
 *
 * This file contains headers of #Plain writer
 * */
#include "writer.h"

#ifndef WAHA_WRITER_PLAIN_H
#define WAHA_WRITER_PLAIN_H

namespace writer {
    /*!
     * \brief Plain message writer to stdout
     *
     * Write each fields in message one-by-one without delimiter
     */
    class Plain : public Writer {

    public:
        virtual void write(zmqpp::message &msg);
    };
}

#endif //WAHA_WRITER_PLAIN_H
