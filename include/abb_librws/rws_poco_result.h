#pragma once

#include <string>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>


namespace abb :: rws
{
  /**
   * \brief A struct for containing the result of a communication.
   */
  struct POCOResult
  {
    /**
     * \brief A constructor.
     * 
     * \param http_status HTTP response status
     * \param header_info HTTP response header info
     * \param content HTTP response content
     */
    POCOResult(Poco::Net::HTTPResponse::HTTPStatus http_status,
      Poco::Net::NameValueCollection const& header_info, std::string const& content);
      

    
    /**
     * \brief Status of the HTTP response.
     */
    Poco::Net::HTTPResponse::HTTPStatus httpStatus() const noexcept
    {
      return httpStatus_;
    }


    /**
     * \brief HTTP response header info.
     */
    auto const& headerInfo() const noexcept
    {
      return headerInfo_;
    }


    /**
     * \brief Content of the HTTP response.
     */
    std::string const& content() const noexcept
    {
      return content_;
    }


  private:
    Poco::Net::HTTPResponse::HTTPStatus httpStatus_;
    std::vector<std::pair<std::string, std::string>> headerInfo_;
    std::string content_;
  };
}