/*!
 * \file writer.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Common writer interface
 *
 * This file contains headers of common writer interface
 * */
#ifndef WAHA_WRITER_H
#define WAHA_WRITER_H

#include <zmqpp/message.hpp>

namespace writer {
    class Writer {
    public:
        /// \brief Write message to output
        /// \param msg ZMQ message
        virtual void write(zmqpp::message &msg) = 0;
    };
}

#endif //WAHA_WRITER_H
