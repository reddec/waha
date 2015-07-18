/*!
 * \file reader.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Common reader interface
 *
 * This file contains headers of common reader interface
 * */

#ifndef WAHA_READER_H
#define WAHA_READER_H

#include <zmqpp/message.hpp>

/*!
 * \brief Abstract builder of zmqpp message
 *
 */
class Reader {
protected:
    std::string target;
    bool verbose = false;
public:

    /// Return verbose flag
    bool isVerbose() const {
        return verbose;
    }

    /// Enable verbose
    void setVerbose(bool verbose) {
        Reader::verbose = verbose;
    }

    /// Get destination service name
    const std::string &getTarget() const {
        return target;
    }

    /// Set destination service name
    void setTarget(const std::string &target) {
        Reader::target = target;
    }

    /// \brief Contsruct zmq message
    /// \return new zmqpp::message
    virtual zmqpp::message read() = 0;
};

#endif //WAHA_READER_H
