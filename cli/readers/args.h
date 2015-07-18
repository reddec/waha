/*!
 * \file args.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Use arguments as message fields
 *
 * This file contains headers of #Args reader
 * */

#include "reader.h"

#ifndef WAHA_ARGS_H
#define WAHA_ARGS_H
namespace reader {
    /*!
     * \brief Read arguments as message fields
     *
     */
    class Args : public Reader {
        std::vector<std::string> args;
    public:

        Args() { }

        Args(const std::vector<std::string> &args) : args(args) { }

        const std::vector<std::string> &getArgs() const;

        void setArgs(const std::vector<std::string> &args);

        /// Build zmqpp message
        virtual zmqpp::message read();
    };
}
#endif //WAHA_ARGS_H
