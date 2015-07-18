/*!
 * \file delimited.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Reader of delimited input
 *
 * This file contains headers of #Delimited reader
 *
 * */
#ifndef WAHA_DELIMITED_H
#define WAHA_DELIMITED_H

#include "reader.h"

namespace reader {
    /*!
     * \brief Read delimited by #Delimited::delimiter input and decode parts if required
     *
     * Supported decoders:
     *  - plain
     *  - base64
     *  - binhex
     */
    class Delimited : public Reader {
    public:
        enum Decoder {
            Plain, BASE64, HEX
        };

    private:
        char delimiter = '\n';
        Decoder decoder = Plain;

        void add_decoded(const std::string &line, zmqpp::message &msg);

    public:


        Delimited() { }

        Delimited(char delimiter, const Decoder &decoder) : delimiter(delimiter), decoder(decoder) { }

        Decoder getDecoder() const;

        void setDecoder(Decoder decoder);

        char getDelimiter() const;

        void setDelimiter(char delimiter);

        virtual zmqpp::message read();

        /// \brief Get decoder by its name
        /// \param name Decoder name hex, base64/b64 or plain
        /// \return HEX, BASE64 or Plain
        static Decoder decoder_by_name(const std::string &name) {
            if (name == "hex") return HEX;
            if (name == "base64" || name == "b64") return BASE64;
            return Plain;
        }
    };
}
#endif //WAHA_DELIMITED_H
