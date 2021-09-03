#include <abb_librws/connection_options.h>


namespace abb :: rws
{
    ConnectionOptions::ConnectionOptions(
        std::string const& ip_address,
        unsigned short port,
        std::string const& username,
        std::string password,
        std::chrono::microseconds connection_timeout,
        std::chrono::microseconds send_timeout,
        std::chrono::microseconds receive_timeout
    )
    :   ip_address {ip_address}
    ,   port {port}
    ,   username {username}
    ,   password {password}
    ,   connection_timeout {connection_timeout}
    ,   send_timeout {send_timeout}
    ,   receive_timeout {receive_timeout}
    {
    }
}