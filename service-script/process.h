/*!
 * \file process.h
 * \date 18.07.15
 * \author RedDec <net.dev@mail.ru>
 * \brief Execute external program as service
 *
 *  This file contains headers of script-service functions.
 *
 *  Main idea is execute external script with arguments from message fields (except RequestID)
 *  and use standard output (optionally with error output too) with return code (optional)
 *  as response fields.
 * */

#ifndef WAHA_PROCESS_H
#define WAHA_PROCESS_H

#include <string>
#include <vector>

/// \brief Connect to broker and start service. Execute external script and process output
/// \param broker_url ZMQ Broker url
/// \param name Published service name
/// \param script Path to executable item (must have exec flag)
/// \param predefined_args Arguments which will be passed to script BEFORE message fields
/// \param with_ret_code Append return code as last message field
/// \param with_stderr Concat stdout with stderr
/// \param disable_stdout Ignore all output of script (stderr also)
/// \param verbose Print request arguments
/// \throw zmqpp::exception on internal ZMQ error
void start_script(const std::string &broker_url,
                  const std::string &name,
                  const std::string &script,
                  const std::vector<std::string> &predefined_args,
                  bool with_ret_code,
                  bool with_stderr,
                  bool disable_stdout,
                  bool verbose);

#endif //WAHA_PROCESS_H
