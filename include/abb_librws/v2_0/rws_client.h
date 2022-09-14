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

#pragma once

#include <Poco/DOM/DOMParser.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPResponse.h>

#include <abb_librws/system_constants.h>
#include <abb_librws/rws_rapid.h>
#include <abb_librws/rws_poco_client.h>
#include <abb_librws/rws_subscription.h>
#include <abb_librws/coordinate.h>
#include <abb_librws/connection_options.h>
#include <abb_librws/v2_0/rws.h>

#include <map>


namespace abb :: rws :: v2_0
{
/**
 * \brief A struct for containing an evaluated communication result.
 */
using RWSResult = Poco::AutoPtr<Poco::XML::Document>;


/**
 * \brief A class for a Robot Web Services (RWS) 2.0 client.
 *
 * Note: Only a subset of the features available in RWS are implemented here.
 *
 * See https://developercenter.robotstudio.com/api/RWS for details about RWS 2.0
 */
class RWSClient
: public SubscriptionManager
{
public:
  /**
   * \brief A struct for containing an evaluated communication result.
   */
  using RWSResult = Poco::AutoPtr<Poco::XML::Document>;

  /**
   * \brief A constructor.
   *
   * \param connection_options RWS connection options.
   *
   * \throw \a RWSError if something goes wrong.
   */
  explicit RWSClient(ConnectionOptions const& connection_options);

  /**
   * \brief Logs out the currently active RWS session.
   */
  ~RWSClient();

  /**
   * \brief Retrieves a list of controller resources (e.g. controller identity and clock information).
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getContollerService();

  /**
   * \brief A method for retrieving the configuration instances of a type, belonging to a specific configuration topic.
   *
   * \param topic specifying the configuration topic.
   * \param type specifying the type in the configuration topic.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getConfigurationInstances(const std::string& topic, const std::string& type);

  /**
   * \brief A method for retrieving all available IO signals on the controller.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getIOSignals();

  /**
   * \brief A method for retrieving the value of an IO signal.
   *
   * \param iosignal for the IO signal's name.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getIOSignal(const std::string& iosignal);

  /**
   * \brief A method for retrieving static information about a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getMechanicalUnitStaticInfo(const std::string& mechunit);

  /**
   * \brief A method for retrieving dynamic information about a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getMechanicalUnitDynamicInfo(const std::string& mechunit);

  /**
   * \brief A method for retrieving the current jointtarget values of a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getMechanicalUnitJointTarget(const std::string& mechunit);

  /**
   * \brief A method for retrieving the current robtarget values of a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   * \param coordinate for the coordinate mode (base, world, tool, or wobj) in which the robtarget will be reported.
   * \param tool for the tool frame relative to which the robtarget will be reported.
   * \param wobj for the work object (wobj) relative to which the robtarget will be reported.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getMechanicalUnitRobTarget(const std::string& mechunit,
                                       Coordinate coordinate = Coordinate::ACTIVE,
                                       const std::string& tool = "",
                                       const std::string& wobj = "");

  /**
   * \brief A method for retrieving info about the current robot controller system.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getRobotWareSystem();

  /**
   * \brief A method for setting the value of an IO signal.
   *
   * \param iosignal for the IO signal's name.
   * \param value for the IO signal's new value.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void setIOSignal(const std::string& iosignal, const std::string& value);

  /**
   * \brief A method for retrieving a file from the robot controller.
   *
   * Note: Depending on the file, then the content can be in text or binary format.
   *
   * \param resource specifying the file's directory and name.
   *
   * \return retrieved file content.
   *
   * \throw \a RWSError if something goes wrong.
   */
  std::string getFile(const FileResource& resource);

  /**
   * \brief A method for uploading a file to the robot controller.
   *
   * \param resource specifying the file's directory and name.
   * \param file_content for the file's content.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void uploadFile(const FileResource& resource, const std::string& file_content);

  /**
   * \brief A method for deleting a file from the robot controller.
   *
   * \param resource specifying the file's directory and name.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void deleteFile(const FileResource& resource);


  /**
   * \brief A method for registering a user as local.
   *
   * \param username specifying the user name.
   * \param application specifying the external application.
   * \param location specifying the location.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void registerLocalUser(const std::string& username = DEFAULT_USERNAME,
                              const std::string& application = SystemConstants::General::EXTERNAL_APPLICATION,
                              const std::string& location = SystemConstants::General::EXTERNAL_LOCATION);

  /**
   * \brief A method for registering a user as remote.
   *
   * \param username specifying the user name.
   * \param application specifying the external application.
   * \param location specifying the location.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void registerRemoteUser(const std::string& username = DEFAULT_USERNAME,
                               const std::string& application = SystemConstants::General::EXTERNAL_APPLICATION,
                               const std::string& location = SystemConstants::General::EXTERNAL_LOCATION);

  // SubscriptionManager implementation
  Poco::Net::WebSocket receiveSubscription(std::string const& subscription_group_id) override;

  /**
   * \brief A method for sending a HTTP GET request and checking response status.
   *
   * \param uri for the URI (path and query).
   * \param accepted_status accepted status values.
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpGet(const std::string& uri,
    std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status = {Poco::Net::HTTPResponse::HTTP_NO_CONTENT, Poco::Net::HTTPResponse::HTTP_OK});

  /**
   * \brief A method for sending a HTTP POST request and checking response status.
   *
   * \param uri for the URI (path and query).
   * \param content for the request's content.
   * \param accepted_status accepted status values.
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpPost(const std::string& uri, const std::string& content = "", const std::string& content_type = "",
    std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status = {Poco::Net::HTTPResponse::HTTP_NO_CONTENT, Poco::Net::HTTPResponse::HTTP_OK});

  /**
   * \brief A method for sending a HTTP PUT request and checking response status.
   *
   * \param uri for the URI (path and query).
   * \param content for the request's content.
   * \param accepted_status accepted status values.
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpPut(const std::string& uri, const std::string& content = "", const std::string& content_type = "",
    std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status = {Poco::Net::HTTPResponse::HTTP_OK, Poco::Net::HTTPResponse::HTTP_CREATED});

  /**
   * \brief A method for sending a HTTP DELETE request and checking response status.
   *
   * \param uri for the URI (path and query).
   * \param accepted_status accepted status values.
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpDelete(const std::string& uri,
    std::set<Poco::Net::HTTPResponse::HTTPStatus> const& accepted_status = {Poco::Net::HTTPResponse::HTTP_OK, Poco::Net::HTTPResponse::HTTP_NO_CONTENT});


private:
  /**
   * \brief Method for parsing a communication result into an XML document.
   *
   * \param result containing the result of the parsing.
   *
   * \return parsed content of \a poco_result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult parseContent(const POCOResult& poco_result);

  /**
   * \brief A method for logging out the currently active RWS session.
   */
  void logout();

  /**
   * \brief Method for generating a configuration URI path.
   *
   * \param topic for the configuration topic.
   * \param type for the configuration type (belonging to the topic).
   *
   * \return std::string containing the path.
   */
  static std::string generateConfigurationPath(const std::string& topic, const std::string& type);

  /**
   * \brief Method for generating an IO signal URI path.
   *
   * \param iosignal for the signal's name.
   *
   * \return std::string containing the path.
   */
  static std::string generateIOSignalPath(const std::string& iosignal);

  /**
   * \brief Method for generating a mechanical unit resource URI path.
   *
   * \param mechunit for the mechanical unit's name.
   *
   * \return std::string containing the path.
   */
  static std::string generateMechanicalUnitPath(const std::string& mechunit);

  /**
   * \brief Method for generating a file resource URI path.
   *
   * \param resource specifying the file's directory and name.
   *
   * \return std::string containing the path.
   */
  static std::string generateFilePath(const FileResource& resource);

  /**
   * \brief Method to verify whether the Post method requires retry
   *
   * \param Http status code returned by client
   *
   * \return bool
   */
  static bool shouldRetryPost(Poco::Net::HTTPResponse::HTTPStatus status);

  ConnectionOptions const connectionOptions_;
  Poco::Net::Context::Ptr context_;
  Poco::Net::HTTPSClientSession session_;
  POCOClient http_client_;
  Poco::XML::DOMParser parser_;
  std::map<std::string, int> mastership_count_;
};
}