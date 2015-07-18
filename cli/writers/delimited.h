/*!
 * \file delimited.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Writer of message fields joined by separator
 *
 * This file contains headers of #Delimited Writer
 * */
#include "writer.h"

#ifndef WAHA_WRITER_DELIMITED_H
#define WAHA_WRITER_DELIMITED_H
namespace writer {
    /*!
     * \brief Writer of message fields joined by separator with encoding each parts if required to:
     * Supported encoders:
     *  - plain
     *  - base64
     *  - binhex
     */
    class Delimited : public Writer {
    public:
        enum Encoder {
            Plain, BASE64, HEX
        };

    private:
        char delimiter = '\n';
        Encoder encoder = Plain;
    public:

        Delimited() { }

        Delimited(char delimiter, const Encoder &encoder) : delimiter(delimiter), encoder(encoder) { }

        char getDelimiter() const;

        void setDelimiter(char delimiter);

        Encoder getEncoder() const;

        void setEncoder(Encoder encoder);

        virtual void write(zmqpp::message &msg);

        /// \brief Get encoder by its name
        /// \param name Encoder name hex, base64/b64 or plain
        /// \return HEX, BASE64 or Plain
        static Encoder encoder_by_name(const std::string &name);
    };
}
#endif //WAHA_DELIMITED_H
