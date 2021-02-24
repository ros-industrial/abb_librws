#pragma once

#include "rws_poco_result.h"

#include <boost/exception/info.hpp>
#include <boost/exception/errinfo_nested_exception.hpp>
#include <boost/exception_ptr.hpp>

#include <stdexcept>


namespace abb :: rws
{
  /**
   * \brief Indicates an RWS error
   */
  class RWSError
  : public std::runtime_error
  , public boost::exception
  {
  protected:
    explicit RWSError(std::string const& message)
    : std::runtime_error {message}
    {
    }
  };


  /**
   * \brief An error occurred when communicating with RWS server.
   */
  class CommunicationError
  : public RWSError
  {
  public:
    explicit CommunicationError(std::string const& message)
    : RWSError {message}
    {
    }
  };


  /**
   * \brief Protocol errors e.g. invalid/faulty HTTP response.
   */
  class ProtocolError
  : public RWSError
  {
  public:
    explicit ProtocolError(std::string const& message)
    : RWSError {message}
    {
    }
  };


  /**
   * \brief Error info containing IO signal name.
   */
  using IoSignalErrorInfo = boost::error_info<struct IoSignalErrorInfoTag, std::string>;


  /**
   * \brief Error info containing HTTPStatus.
   */
  using HttpStatusErrorInfo = boost::error_info<struct HttpStatusErrorInfoTag, Poco::Net::HTTPResponse::HTTPStatus>;


  /**
   * \brief Error info containing an HTTP method.
   */
  using HttpMethodErrorInfo = boost::error_info<struct HttpMethodErrorInfoTag, std::string>;


  /**
   * \brief Error info containing HTTP request content.
   */
  using HttpRequestContentErrorInfo = boost::error_info<struct HttpRequestContentErrorInfoTag, std::string>;


  /**
   * \brief Error info containing an HTTP response.
   */
  using HttpResponseErrorInfo = boost::error_info<struct HttpResponseErrorInfoTag, POCOResult>;


  /**
   * \brief Error info containing an URI.
   */
  using UriErrorInfo = boost::error_info<struct UriErrorInfoTag, std::string>;
}
