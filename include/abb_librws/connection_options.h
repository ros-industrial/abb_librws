#pragma once

#include "system_constants.h"

#include <string>
#include <chrono>


namespace abb :: rws
{
    struct ConnectionOptions
    {
        /// \brief Robot controller's IP address.
        std::string ip_address;

        /// \brief port used by the RWS server.
        unsigned short port = SystemConstants::General::DEFAULT_PORT_NUMBER;

        /// \brief username to the RWS authentication process.
        std::string username = SystemConstants::General::DEFAULT_USERNAME;

        /// \brief password to the RWS authentication process.
        std::string password = SystemConstants::General::DEFAULT_PASSWORD;

        /// \brief HTTP connection timeout
        std::chrono::microseconds connection_timeout = std::chrono::milliseconds {400};

        /// \brief HTTP send timeout
        std::chrono::microseconds send_timeout = std::chrono::milliseconds {400};

        /// \brief HTTP receive timeout
        std::chrono::microseconds receive_timeout = std::chrono::milliseconds {400};
    };
}
