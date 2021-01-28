#pragma once

#include <string>


namespace abb :: rws
{
    /**
     * \brief A struct for containing WebSocket info.
     */
    struct WebSocketInfo
    {
        /**
         * \brief Flags from a received WebSocket frame.
         */
        int flags = 0;

        /**
         * \brief Content from a received WebSocket frame.
         */
        std::string frame_content;
    };
}