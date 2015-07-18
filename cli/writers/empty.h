/*!
 * \file empty.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Print nothing from message
 *
 * This file contains headers and implementation of #Empty writer
 * */
#include "writer.h"

#ifndef WAHA_WRITER_EMPTY_H
#define WAHA_WRITER_EMPTY_H
namespace writer {
    /*!
     * \brief Print nothing from message
     *
     *  Works same as > /dev/null
     */
    class Empty : public Writer {

    public:
        virtual void write(zmqpp::message &) {
            //Do nothing
        }
    };
}
#endif //WAHA_EMPTY_H
