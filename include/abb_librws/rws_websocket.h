#pragma once

#include "rws_poco_client.h"

#include <Poco/Net/WebSocket.h>

#include <string>
#include <memory>
#include <mutex>


namespace abb :: rws
{
    /**
     * \brief WebSocket frame.
     */
    struct WebSocketFrame
    {
        /**
         * \brief WebSocket frame flags.
         */
        int flags = 0;

        /**
         * \brief WebSocket frame content.
         */
        std::string frame_content;
    };


    /**
     * \brief A method to map the flags of a received WebSocket frame to a string representing the opcode.
     *
     * \return std::string containing the mapped opcode.
     */
    std::string mapWebSocketOpcode(int flags);


    /**
     * \brief Manages a WebSocket connection.
     */
    class WebSocket
    {
    public:
        /**
         * \brief Connects to a WebSocket.
         *
         * \param client \a POCOClient for HTTP request.
         * \param uri for the URI (path and query).
         * \param protocol for the WebSocket protocol.
         * \param timeout for the WebSocket communication timeout [microseconds].
         *
         * \return Pointer to a client WebSocket.
         * 
         * \throw \a std::runtime_error if something goes wrong
         */
        WebSocket(POCOClient& client, std::string const& uri, std::string const& protocol, Poco::Int64 timeout);

        ~WebSocket();


        /**
         * \brief A method for receiving a WebSocket frame.
         * 
         * \brief frame the received frame
         *
         * \return true if the connection is still active, false otherwise.
         */
        bool receiveFrame(WebSocketFrame& frame);


        /**
         * \brief Forcibly shut down the websocket connection.
         *
         * The connection is shut down immediately.
         * Subsequently, the function will block until a current call to receiveFrame() has finished,
         * before cleaning up the local state.
         *
         * Note that since mutexes do not guarantee the order of acquisition for multiple contenders,
         * it is undefined how many calls to receiveFrame() will still attempt to use the shut down
         * connection before the local state is cleaned. Those invocation will throw a runtime error.
         */
        void shutdown();


    private:
        /**
         * \brief Static constant for the socket's buffer size.
         */
        static const size_t BUFFER_SIZE = 1024;

        /**
         * \brief A buffer for a Subscription.
         */
        char websocket_buffer_[BUFFER_SIZE];

        /**
         * \brief A mutex for protecting the client's WebSocket resources.
         *
         * This mutex must be held while using the p_websocket_ member,
         * and while invalidating the pointer.
         */
        std::mutex websocket_use_mutex_;

        /**
         * \brief A pointer to a Subscription client.
         */
        std::unique_ptr<Poco::Net::WebSocket> p_websocket_;
    };
}