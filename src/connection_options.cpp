#include <abb_librws/connection_options.h>

#include <utility>


namespace abb :: rws
{
    ConnectionOptions::ConnectionOptions(
        std::string  ip_address,
        unsigned short port,
        std::string  username,
        std::string password,
        std::chrono::microseconds connection_timeout,
        std::chrono::microseconds send_timeout,
        std::chrono::microseconds receive_timeout
    )
    :   ip_address {std::move(ip_address)}
    ,   port {port}
    ,   username {std::move(username)}
    ,   password {std::move(password)}
    ,   connection_timeout {connection_timeout}
    ,   send_timeout {send_timeout}
    ,   receive_timeout {receive_timeout}
    {
    }
}