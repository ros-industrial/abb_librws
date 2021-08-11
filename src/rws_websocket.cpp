#include <Poco/Net/WebSocket.h>

#include <abb_librws/rws_websocket.h>


namespace abb :: rws
{
  std::string mapWebSocketOpcode(int flags)
  {
    using Poco::Net::WebSocket;

    std::string result;

    switch (flags & WebSocket::FRAME_OP_BITMASK)
    {
      case WebSocket::FRAME_OP_CONT:
        result = "FRAME_OP_CONT";
      break;

      case WebSocket::FRAME_OP_TEXT:
        result = "FRAME_OP_TEXT";
      break;

      case WebSocket::FRAME_OP_BINARY:
        result = "FRAME_OP_BINARY";
      break;

      case WebSocket::FRAME_OP_CLOSE:
        result = "FRAME_OP_CLOSE";
      break;

      case WebSocket::FRAME_OP_PING:
        result = "FRAME_OP_PING";
      break;

      case WebSocket::FRAME_OP_PONG:
        result = "FRAME_OP_PONG";
      break;

      default:
        result = "FRAME_OP_UNDEFINED";
      break;
    }

    return result;
  }
}