#pragma once

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <string>


namespace abb :: rws
{
  /**
   * \brief A struct for containing the result of a communication.
   */
  struct POCOResult
  {
    /**
     * \brief An enum for specifying a general status.
     */
    enum GeneralStatus
    {
      UNKNOWN,                         ///< Unknown status.
      OK,                              ///< Ok status.
      WEBSOCKET_NOT_ALLOCATED,         ///< The WebSocket has not been allocated.
      EXCEPTION_POCO_INVALID_ARGUMENT, ///< POCO invalid argument exception
      EXCEPTION_POCO_TIMEOUT,          ///< POCO timeout exception.
      EXCEPTION_POCO_NET,              ///< POCO net exception.
      EXCEPTION_POCO_WEBSOCKET         ///< POCO WebSocket exception.
    };

    /**
     * \brief A struct for containing POCO info.
     */
    struct POCOInfo
    {
      /**
       * \brief A struct for containing HTTP info.
       */
      struct HTTPInfo
      {
        /**
         * \brief A struct for containing HTTP request info.
         */
        struct RequestInfo
        {
          /**
           * \brief Method used for the request.
           */
          std::string method;

          /**
           * \brief URI used for the request.
           */
          std::string uri;

          /**
           * \brief Content used for the request.
           */
          std::string content;
        };

        /**
         * \brief A struct for containing HTTP response info.
         */
        struct ResponseInfo
        {
          /**
           * \brief Response status.
           */
          Poco::Net::HTTPResponse::HTTPStatus status;

          /**
           * \brief Response header info.
           */
          std::string header_info;

          /**
           * \brief Response content.
           */
          std::string content;

          /**
           * \brief A default constructor.
           */
          ResponseInfo() : status(Poco::Net::HTTPResponse::HTTP_OK) {}
        };

        /**
         * \brief Info about a HTTP request.
         */
        RequestInfo request;

         /**
          * \brief Info about a HTTP response.
          */
        ResponseInfo response;
      };

      /**
       * \brief Container for HTTP info.
       */
      HTTPInfo http;
    };

    /**
     * \brief Container for a general status.
     */
    GeneralStatus status;

    /**
     * \brief Container for an exception message (if one occurred).
     */
    std::string exception_message;

    /**
     * \brief Container for POCO info.
     */
    POCOInfo poco_info;

    /**
     * \brief A default constructor.
     */
    POCOResult() : status(UNKNOWN) {};

    /**
     * \brief A method for adding info from a HTTP request.
     *
     * \param request for the HTTP request.
     * \param request_content for the HTTP request's content.
     */
    void addHTTPRequestInfo(const Poco::Net::HTTPRequest& request, const std::string& request_content = "");

    /**
     * \brief A method for adding info from a HTTP response.
     *
     * \param response for the HTTP response.
     * \param response_content for the HTTP response's content.
     */
    void addHTTPResponseInfo(const Poco::Net::HTTPResponse& response, const std::string& response_content = "");

    /**
     * \brief A method to map the general status to a std::string.
     *
     * \return std::string containing the mapped general status.
     */
    std::string mapGeneralStatus() const;

    /**
     * \brief A method to construct a text representation of the result.
     *
     * \param verbose indicating if the log text should be verbose or not.
     * \param indent for indentation.
     *
     * \return std::string containing the text representation.
     */
    std::string toString(const bool verbose = false, const size_t indent = 0) const;
  };
}