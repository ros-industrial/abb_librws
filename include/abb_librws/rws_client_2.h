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

#include "system_constants.h"
#include "rws_rapid.h"
#include "rws_poco_client.h"
#include "rws_resource.h"
#include "rws_subscription.h"
#include "coordinate.h"
#include "mastership.h"

#include <map>


namespace abb
{
namespace rws
{
/**
 * \brief A class for a Robot Web Services (RWS) 2.0 client.
 *
 * Note: Only a subset of the features available in RWS are implemented here.
 *
 * See https://developercenter.robotstudio.com/api/RWS for details about RWS 2.0
 */
class RWSClient2
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
   * \param ip_address specifying the robot controller's IP address.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSClient2(const std::string& ip_address);

  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   * \param username for the username to the RWS authentication process.
   * \param password for the password to the RWS authentication process.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSClient2(const std::string& ip_address, const std::string& username, const std::string& password);

  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   * \param port for the port used by the RWS server.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSClient2(const std::string& ip_address, const unsigned short port);

  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   * \param port for the port used by the RWS server.
   * \param username for the username to the RWS authentication process.
   * \param password for the password to the RWS authentication process.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSClient2(const std::string& ip_address,
            const unsigned short port,
            const std::string& username,
            const std::string& password);

  /**
   * \brief Logs out the currently active RWS session.
   */
  ~RWSClient2();

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
   * \brief A method for retrieving the data of a RAPID symbol.
   *
   * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getRAPIDSymbolData(const RAPIDResource& resource);

  /**
   * \brief A method for retrieving the data of a RAPID symbol (parsed into a struct representing the RAPID data).
   *
   * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
   * \param data for containing the retrieved data.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void getRAPIDSymbolData(const RAPIDResource& resource, RAPIDSymbolDataAbstract& data);

  /**
   * \brief A method for retrieving the properties of a RAPID symbol.
   *
   * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getRAPIDSymbolProperties(const RAPIDResource& resource);

  /**
   * \brief A method for retrieving the execution state of RAPID.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getRAPIDExecution();

  /**
   * \brief A method for retrieving information about the RAPID modules of a RAPID task.
   *
   * \param task specifying the RAPID task.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getRAPIDModulesInfo(const std::string& task);

  /**
   * \brief A method for retrieving the RAPID tasks that are defined in the robot controller system.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getRAPIDTasks();

  /**
   * \brief A method for retrieving info about the current robot controller system.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getRobotWareSystem();

  /**
   * \brief A method for retrieving the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getSpeedRatio();

  /**
   * \brief A method for retrieving the controller state.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getPanelControllerState();

  /**
   * \brief A method for retrieving the operation mode of the controller.
   *
   * \return RWSResult containing the result.
   *
   * \throw \a RWSError if something goes wrong.
   */
  RWSResult getPanelOperationMode();

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
   * \brief A method for setting the data of a RAPID symbol.
   *
   * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
   * \param data for the RAPID symbol's new data.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void setRAPIDSymbolData(const RAPIDResource& resource, const std::string& data);

  /**
   * \brief A method for setting the data of a RAPID symbol (based on the provided struct representing the RAPID data).
   *
   * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
   * \param data for the RAPID symbol's new data.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void setRAPIDSymbolData(const RAPIDResource& resource, const RAPIDSymbolDataAbstract& data);

  /**
   * \brief A method for starting RAPID execution in the robot controller.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void startRAPIDExecution();

  /**
   * \brief A method for stopping RAPID execution in the robot controller.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void stopRAPIDExecution();

  /**
   * \brief A method for reseting the RAPID program pointer in the robot controller.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void resetRAPIDProgramPointer();

  /**
   * \brief A method for turning on the robot controller's motors.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void setMotorsOn();

  /**
   * \brief A method for turning off the robot controller's motors.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void setMotorsOff();

  /**
   * \brief A method for setting the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
   *
   * Note: The ratio must be an integer in the range [0, 100] (ie: inclusive).
   *
   * \param ratio specifying the new ratio.
   *
   * \return RWSResult containing the result.
   *
   * \throw std::out_of_range if argument is out of range.
   * \throw std::runtime_error if failed to create a string from the argument.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void setSpeedRatio(unsigned int ratio);

  /**
   * \brief A method for loading a module to the robot controller.
   *
   * \param task specifying the RAPID task.
   * \param resource specifying the file's directory and name.
   * \param replace indicating if the actual module into the controller must be replaced by the new one or not.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void loadModuleIntoTask(const std::string& task, const FileResource& resource, const bool replace = false);

  /**
   * \brief A method for unloading a module to the robot controller.
   *
   * \param task specifying the RAPID task.
   * \param resource specifying the file's directory and name.
   *
   * \throw \a RWSError if something goes wrong.
   */
  void unloadModuleFromTask(const std::string& task, const FileResource& resource);

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
  void registerLocalUser(const std::string& username = SystemConstants::General::DEFAULT_USERNAME,
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
  void registerRemoteUser(const std::string& username = SystemConstants::General::DEFAULT_USERNAME,
                               const std::string& application = SystemConstants::General::EXTERNAL_APPLICATION,
                               const std::string& location = SystemConstants::General::EXTERNAL_LOCATION);

  /**
   * \brief A method for setting the HTTP communication timeout.
   *
   * \note This method resets the internal HTTP client session, causing the
   *       RWS server (robot controller) to send a new cookie. The RWS
   *       session id is not changed.
   *
   * \param timeout for the HTTP communication timeout.
   */
  void setHTTPTimeout(Poco::Timespan timeout);

  /**
   * \brief Get HTTP receive timeout.
   *
   * \return HTTP receive timeout.
   */
  Poco::Timespan getHTTPTimeout() const noexcept;

  /**
   * @brief Request mastership of given type
   *
   * @param type type of requested mastership
   */
  void requestMastership(std::string const& type);

  /**
   * @brief Release mastership of given type
   *
   * @param type type of mastership to be released
   */
  void releaseMastership(std::string const& type);


  // SubscriptionManager implementation
  std::string openSubscription(std::vector<std::pair<std::string, SubscriptionPriority>> const& resources) override;
  void closeSubscription(std::string const& subscription_group_id) override;
  Poco::Net::WebSocket receiveSubscription(std::string const& subscription_group_id) override;
  std::string getResourceURI(IOSignalResource const& io_signal) const override;
  std::string getResourceURI(RAPIDResource const& resource) const override;


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
   * \brief A method for sending a HTTP GET request and checking response status.
   *
   * \param uri for the URI (path and query).
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpGet(const std::string& uri);

  /**
   * \brief A method for sending a HTTP POST request and checking response status.
   *
   * \param uri for the URI (path and query).
   * \param content for the request's content.
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpPost(const std::string& uri, const std::string& content = "", const std::string& content_type = "");

  /**
   * \brief A method for sending a HTTP PUT request and checking response status.
   *
   * \param uri for the URI (path and query).
   * \param content for the request's content.
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpPut(const std::string& uri, const std::string& content = "", const std::string& content_type = "");

  /**
   * \brief A method for sending a HTTP DELETE request and checking response status.
   *
   * \param uri for the URI (path and query).
   *
   * \return POCOResult containing the result.
   */
  POCOResult httpDelete(const std::string& uri);


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
   * \brief Method for generating a RAPID data resource URI path.
   *
   * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
   *
   * \return std::string containing the path.
   */
  static std::string generateRAPIDDataPath(const RAPIDResource& resource);

  /**
   * \brief Method for generating a RAPID properties resource URI path.
   *
   * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
   *
   * \return std::string containing the path.
   */
  static std::string generateRAPIDPropertiesPath(const RAPIDResource& resource);

  /**
   * \brief Method for generating a file resource URI path.
   *
   * \param resource specifying the file's directory and name.
   *
   * \return std::string containing the path.
   */
  static std::string generateFilePath(const FileResource& resource);

  /**
   * \brief Method for generating a task resource URI path.
   *
   * \param task for the task name.
   *
   * \return std::string containing the path.
   */
  std::string generateRAPIDTasksPath(const std::string& task);

  Poco::Net::Context::Ptr context_;
  Poco::Net::HTTPSClientSession session_;
  POCOClient http_client_;
  Poco::XML::DOMParser parser_;
  std::map<std::string, int> mastership_count_;
};

} // end namespace rws
} // end namespace abb
