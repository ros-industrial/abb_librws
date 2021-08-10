/***********************************************************************************************************************
 *
 * Copyright (c) 2015, ABB Schweiz AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that
 * the following conditions are met:
 *
 *    * Redistributions of source code must retain the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer.
 *    * Redistributions in binary form must reproduce the
 *      above copyright notice, this list of conditions
 *      and the following disclaimer in the documentation
 *      and/or other materials provided with the
 *      distribution.
 *    * Neither the name of ABB nor the names of its
 *      contributors may be used to endorse or promote
 *      products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***********************************************************************************************************************
 */

#ifndef RWS_POCO_CLIENT_H
#define RWS_POCO_CLIENT_H

#include <abb_librws/rws_poco_result.h>

#include <Poco/Mutex.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPCredentials.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>

#include <deque>
#include <optional>

namespace abb
{
namespace rws
{
  /**
   * \brief A class for a simple client based on POCO.
   */
  class POCOClient
  {
  public:
    /**
     * \brief A constructor.
     *
     * \param session HTTP session. This can be either an HTTPClientSession or an HTTPSClientSession,
     *  depending on which protocol should be used.
     * \param username for the username to the remote server's authentication process.
     * \param password for the password to the remote server's authentication process.
     */
    POCOClient(Poco::Net::HTTPClientSession& session,
              const std::string& username,
              const std::string& password);

    /**
     * \brief A destructor.
     */
    ~POCOClient();

    /**
     * \brief A method for sending a HTTP GET request.
     *
     * \param uri for the URI (path and query).
     *
     * \return POCOResult containing the result.
     */
    POCOResult httpGet(const std::string& uri);

    /**
     * \brief A method for sending a HTTP POST request.
     *
     * \param uri for the URI (path and query).
     * \param content for the request's content.
     *
     * \return POCOResult containing the result.
     */
    POCOResult httpPost(const std::string& uri, const std::string& content = "", const std::string& content_type = "");

    /**
     * \brief A method for sending a HTTP PUT request.
     *
     * \param uri for the URI (path and query).
     * \param content for the request's content.
     *
     * \return POCOResult containing the result.
     */
    POCOResult httpPut(const std::string& uri, const std::string& content = "", const std::string& content_type = "");

    /**
     * \brief A method for sending a HTTP DELETE request.
     *
     * \param uri for the URI (path and query).
     *
     * \return POCOResult containing the result.
     */
    POCOResult httpDelete(const std::string& uri);

    /**
     * \brief A method for connecting a WebSocket.
     *
     * \param uri for the URI (path and query).
     * \param protocol for the WebSocket protocol.
     *
     * \return Newly created client WebSocket.
     *
     * \throw \a std::runtime_error if something goes wrong
     */
    Poco::Net::WebSocket webSocketConnect(const std::string& uri, const std::string& protocol, Poco::Net::HTTPClientSession& session);


    /**
     * \brief Method for retrieving the internal log as a text string.
     *
     * \param verbose indicating if the log text should be verbose or not.
     *
     * \return std::string containing the log text. An empty text string is returned if the log is empty.
     */
    std::string getLogText(bool verbose = false) const;


    /**
     * \brief Method for retrieving only the most recently logged event as a text string.
     *
     * \param verbose indicating if the log text should be verbose or not.
     *
     * \return std::string containing the log text. An empty text string is returned if the log is empty.
     */
    std::string getLogTextLatestEvent(bool verbose = false) const;


  private:
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
      };

      /**
       * \brief Info about a HTTP request.
       */
      std::optional<RequestInfo> request;

        /**
        * \brief Info about a HTTP response.
        */
      std::optional<ResponseInfo> response;


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
       * \brief A method to construct a text representation of the result.
       *
       * \param verbose indicating if the log text should be verbose or not.
       * \param indent for indentation.
       *
       * \return std::string containing the text representation.
       */
      std::string toString(bool verbose = false, size_t indent = 0) const;
    };


    /**
     * \brief A method for making a HTTP request.
     *
     * \param method for the request's method.
     * \param uri for the URI (path and query).
     * \param content for the request's content.
     *
     * \return POCOResult containing the result.
     */
    POCOResult makeHTTPRequest(const std::string& method,
                              const std::string& uri = "/",
                              const std::string& content = "",
                              const std::string& content_type = "");

    /**
     * \brief A method for sending and receiving HTTP messages.
     *
     * \param request for the HTTP request.
     * \param response for the HTTP response.
     * \param request_content for the request's content.
     * \param response_content for the response content.
     */
    void sendAndReceive(Poco::Net::HTTPRequest& request,
                        Poco::Net::HTTPResponse& response,
                        const std::string& request_content,
                        std::string& response_content);

    /**
     * \brief A method for performing authentication.
     *
     * \param request for the HTTP request.
     * \param response for the HTTP response.
     * \param request_content for the request's content.
     * \param response_content for the response content.
     */
    void authenticate(Poco::Net::HTTPRequest& request,
                      Poco::Net::HTTPResponse& response,
                      const std::string& request_content,
                      std::string& response_content);

    /**
     * \brief A method for extracting and storing information from a cookie string.
     *
     * \param cookie_string for the cookie string.
     */
    void extractAndStoreCookie(const std::string& cookie_string);

    /**
     * \brief Static constant for the default HTTP communication timeout [microseconds].
     */
    static const Poco::Int64 DEFAULT_HTTP_TIMEOUT = 400e3;


    /**
     * \brief A mutex for protecting the clients's HTTP resources.
     */
    Poco::Mutex http_mutex_;

    /**
     * \brief A HTTP client session.
     */
    Poco::Net::HTTPClientSession& http_client_session_;

    /**
     * \brief HTTP credentials for the remote server's access authentication process.
     */
    Poco::Net::HTTPCredentials http_credentials_;

    /**
     * \brief A container for cookies received from a server.
     */
    Poco::Net::NameValueCollection cookies_;

    /**
     * \brief Static constant for the log's size.
     */
    static const size_t LOG_SIZE = 20;

    /**
     * \brief Container for logging communication results.
     */
    std::deque<HTTPInfo> log_;
  };
} // end namespace rws
} // end namespace abb

#endif
