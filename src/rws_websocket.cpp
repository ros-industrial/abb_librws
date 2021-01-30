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


  WebSocket::WebSocket(POCOClient& client, std::string const& uri, std::string const& protocol, Poco::Int64 timeout)
  : p_websocket_ {client.webSocketConnect(uri, protocol)}
  {
    p_websocket_->setReceiveTimeout(Poco::Timespan(timeout));
  }


  WebSocket::~WebSocket()
  {
    shutdown();
  }


  bool WebSocket::receiveFrame(WebSocketFrame& frame)
  {
    using Poco::Net::WebSocket;

    // Lock the object's mutex. It is released when the method goes out of scope.
    std::lock_guard<std::mutex> lock {websocket_use_mutex_};

    // If the connection is still active...
    if (p_websocket_)
    {
      int flags = 0;
      std::string content;

      // Wait for (non-ping) WebSocket frames.
      do
      {
        flags = 0;
        int number_of_bytes_received = p_websocket_->receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
        content = std::string(websocket_buffer_, number_of_bytes_received);

        // Check for ping frame.
        if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING)
        {
          // Reply with a pong frame.
          p_websocket_->sendFrame(websocket_buffer_,
                                  number_of_bytes_received,
                                  WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
        }
      } while ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING);

      // Check for closing frame.
      if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE)
      {
        // Do not pass content of a closing frame to end user,
        // according to "The WebSocket Protocol" RFC6455.
        content.clear();

        // Shutdown the WebSocket.
        p_websocket_->shutdown();

        // Destroy the WebSocket to indicate that the connection is shut down.
        p_websocket_.reset();
      }

      frame.flags = flags;
      frame.frame_content = content;
    }

    return (bool)p_websocket_;
  }


  void WebSocket::shutdown()
  {
    if (p_websocket_)
    {
      // Shut down the socket. This should make receiveFrame() return as soon as possible.
      p_websocket_->shutdown();

      // Also acquire the websocket lock before invalidating the pointer,
      // or we will break running calls to receiveFrame().
      std::lock_guard<std::mutex> use_lock {websocket_use_mutex_};
      p_websocket_.reset();
    }
  }
}