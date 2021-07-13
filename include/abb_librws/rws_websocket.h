#pragma once

#include <string>


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
}