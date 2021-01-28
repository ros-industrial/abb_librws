#include <abb_librws/rws_poco_result.h>

#include <Poco/Net/WebSocket.h>

#include <sstream>


using namespace Poco;
using namespace Poco::Net;


namespace abb :: rws
{
  /***********************************************************************************************************************
   * Struct definitions: POCOResult
   */

  /************************************************************
   * Primary methods
   */

  void POCOResult::addHTTPRequestInfo(const Poco::Net::HTTPRequest& request,
                                                  const std::string& request_content)
  {
    poco_info.http.request.method = request.getMethod();
    poco_info.http.request.uri = request.getURI();
    poco_info.http.request.content = request_content;
  }

  void POCOResult::addHTTPResponseInfo(const Poco::Net::HTTPResponse& response,
                                                  const std::string& response_content)
  {
    std::string header_info;

    for (HTTPResponse::ConstIterator i = response.begin(); i != response.end(); ++i)
    {
      header_info += i->first + "=" + i->second + "\n";
    }

    poco_info.http.response.status = response.getStatus();
    poco_info.http.response.header_info = header_info;
    poco_info.http.response.content = response_content;
  }

  void POCOResult::addWebSocketFrameInfo(const int flags,
                                                    const std::string& frame_content)
  {
    poco_info.websocket.flags = flags;
    poco_info.websocket.frame_content = frame_content;
  }

  /************************************************************
   * Auxiliary methods
   */

  std::string POCOResult::mapGeneralStatus() const
  {
    std::string result;

    switch (status)
    {
      case POCOResult::UNKNOWN:
        result = "UNKNOWN";
      break;

      case POCOResult::OK:
        result = "OK";
      break;

      case POCOResult::WEBSOCKET_NOT_ALLOCATED:
        result = "WEBSOCKET_NOT_ALLOCATED";
      break;

      case POCOResult::EXCEPTION_POCO_INVALID_ARGUMENT:
        result = "EXCEPTION_POCO_INVALID_ARGUMENT";
      break;

      case POCOResult::EXCEPTION_POCO_TIMEOUT:
        result = "EXCEPTION_POCO_TIMEOUT";
      break;

      case POCOResult::EXCEPTION_POCO_NET:
        result = "EXCEPTION_POCO_NET";
      break;

      case POCOResult::EXCEPTION_POCO_WEBSOCKET:
        result = "EXCEPTION_POCO_WEBSOCKET";
      break;

      default:
        result = "UNDEFINED";
      break;
    }

    return result;
  }

  std::string POCOResult::mapWebSocketOpcode() const
  {
    std::string result;

    switch (poco_info.websocket.flags & WebSocket::FRAME_OP_BITMASK)
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

  std::string POCOResult::toString(const bool verbose, const size_t indent) const
  {
    std::stringstream ss;

    std::string seperator = (indent == 0 ? " | " : "\n" + std::string(indent, ' '));

    ss << "General status: " << mapGeneralStatus();

    if (!poco_info.http.request.method.empty())
    {
      ss << seperator << "HTTP Request: " << poco_info.http.request.method << " " << poco_info.http.request.uri;

      if (status == OK)
      {
        ss << seperator << "HTTP Response: " << poco_info.http.response.status << " - "
          << HTTPResponse::getReasonForStatus(poco_info.http.response.status);

        if (verbose)
        {
          ss << seperator << "HTTP Response Content: " << poco_info.http.response.content;
        }
      }
    }
    else if (status == OK)
    {
      ss << seperator << "WebSocket frame: " << mapWebSocketOpcode();
    }

    return ss.str();
  }
}