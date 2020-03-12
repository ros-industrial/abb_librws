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

#ifndef RWS_INTERFACE_H
#define RWS_INTERFACE_H

#include "rws_client.h"

namespace abb
{
namespace rws
{
/**
 * \brief A class for wrapping a Robot Web Services (RWS) client in a more user friendly interface.
 */
class RWSInterface
{
public:
  /**
   * \brief A struct for containing system information of the robot controller.
   */
  struct SystemInfo
  {
    /**
     * \brief The RobotWare version name.
     */
    std::string robot_ware_version;

    /**
     * \brief The system's name.
     */
    std::string system_name;
  };

  /**
   * \brief A struct for containing information about a RobotWare option.
   */
  struct RobotWareOptionInfo
  {
    /**
     * \brief A constructor.
     *
     * \param name for the name of the option.
     * \param description for the description of the option.
     */
    RobotWareOptionInfo(const std::string& name, const std::string& description)
    :
    name(name),
    description(description)
    {}

    /**
     * \brief The option's name.
     */
    std::string name;

    /**
     * \brief The options's description.
     */
    std::string description;
  };

  /**
   * \brief A struct for containing information about a RAPID module.
   */
  struct RAPIDModuleInfo
  {
    /**
     * \brief A constructor.
     *
     * \param name for the name of the module.
     * \param type for the type of the module.
     */
    RAPIDModuleInfo(const std::string& name, const std::string& type)
    :
    name(name),
    type(type)
    {}

    /**
     * \brief The module's name.
     */
    std::string name;

    /**
     * \brief The module's type.
     */
    std::string type;
  };

  /**
   * \brief A struct for containing information about a RAPID task.
   */
  struct RAPIDTaskInfo
  {
    /**
     * \brief A constructor.
     *
     * \param name for the name of the task.
     * \param is_motion_task indicating if the task is a motion task or not.
     */
    RAPIDTaskInfo(const std::string& name, bool is_motion_task)
    :
    name(name),
    is_motion_task(is_motion_task)
    {}

    /**
     * \brief The task's name.
     */
    std::string name;

    /**
     * \brief Flag indicating if the task is a motion task.
     */
    bool is_motion_task;
  };

  /**
   * \brief A struct for containing static information (at least during runtime) about the robot controller.
   */
  struct StaticInfo
  {
    /**
     * \brief Information about the defined RAPID tasks.
     */
    std::vector<RAPIDTaskInfo> rapid_tasks;

    /**
     * \brief System information.
     */
    SystemInfo system_info;
  };

  /**
   * \brief A struct for containing runtime information about the robot controller.
   */
  struct RuntimeInfo
  {
    /**
     * \brief A default constructor.
     */
    RuntimeInfo() : rws_connected(false) {}

    /**
     * \brief Indicator for if the mode is auto or not or unknown.
     */
    TriBool auto_mode;

    /**
     * \brief Indicator for if the motors are on or not or unknown.
     */
    TriBool motors_on;

    /**
     * \brief Indicator for if RAPID is running or not or unknown.
     */
    TriBool rapid_running;

    /**
     * \brief Indicator for if RWS is connected to the robot controller system.
     */
    bool rws_connected;
  };

  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   */
  RWSInterface(const std::string& ip_address)
  :
  rws_client_(ip_address,
              SystemConstants::General::DEFAULT_PORT_NUMBER,
              SystemConstants::General::DEFAULT_USERNAME,
              SystemConstants::General::DEFAULT_PASSWORD)
  {}

  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   * \param username for the username to the RWS authentication process.
   * \param password for the password to the RWS authentication process.
   */
  RWSInterface(const std::string& ip_address, const std::string& username, const std::string& password)
  :
  rws_client_(ip_address,
              SystemConstants::General::DEFAULT_PORT_NUMBER,
              username,
              password)
  {}

  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   * \param port for the port used by the RWS server.
   */
  RWSInterface(const std::string& ip_address, const unsigned short port)
  :
  rws_client_(ip_address,
              port,
              SystemConstants::General::DEFAULT_USERNAME,
              SystemConstants::General::DEFAULT_PASSWORD)
  {}

  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   * \param port for the port used by the RWS server.
   * \param username for the username to the RWS authentication process.
   * \param password for the password to the RWS authentication process.
   */
  RWSInterface(const std::string& ip_address,
               const unsigned short port,
               const std::string& username,
               const std::string& password)
  :
  rws_client_(ip_address,
              port,
              username,
              password)
  {}

  /**
   * \brief A method for collecting runtime information of the robot controller.
   *
   * \return RuntimeInfo containing the runtime information.
   */
  RuntimeInfo collectRuntimeInfo();

  /**
   * \brief A method for collecting static information (at least during runtime) of the robot controller.
   *
   * \return StaticInfo containing the static information (at least during runtime).
   */
  StaticInfo collectStaticInfo();

  /**
   * \brief Retrieves the configuration instances for the present (RobotWare) options.
   *
   * \return std::vector<cfg::sys::PresentOption> containing a list of the present (RobotWare) options.
   */
  std::vector<cfg::sys::PresentOption> getCFGPresentOptions();

  /**
   * \brief A method for retrieving the RobotWare options present on the active robot controller system.
   *
   * \return std::vector<RobotWareOptionInfo> containing a list of the present RobotWare options.
   */
  ABB_LIBRWS_DEPRECATED std::vector<RobotWareOptionInfo> getPresentRobotWareOptions();

  /**
   * \brief A method for retrieving the value if an IO signal.
   *
   * \param iosignal for the name of the IO signal.
   *
   * \return std::string containing the IO signal's value (empty if not found).
   */
  std::string getIOSignal(const std::string& iosignal);

  /**
   * \brief A method for retrieving the current jointtarget values of a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   * \param p_jointtarget for storing the retrieved jointtarget data.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "correct parsing".
   */
  bool getMechanicalUnitJointTarget(const std::string& mechunit, JointTarget* p_jointtarget);

  /**
   * \brief A method for retrieving the current robtarget values of a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   * \param p_robtarget for storing the retrieved robtarget data.
   * \param coordinate for the coordinate mode (base, world, tool, or wobj) in which the robtarget will be reported.
   * \param tool for the tool frame relative to which the robtarget will be reported.
   * \param wobj for the work object (wobj) relative to which the robtarget will be reported.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "correct parsing".
   */
  bool getMechanicalUnitRobTarget(const std::string& mechunit,
                                  RobTarget* p_robtarget,
                                  const RWSClient::Coordinate& coordinate = RWSClient::ACTIVE,
                                  const std::string& tool = "",
                                  const std::string& wobj = "");

  /**
   * \brief A method for retrieving the data of a RAPID symbol (parsed into a struct representing the RAPID data).
   *
   * \param task for the name of the RAPID task containing the RAPID symbol.
   * \param module for the name of the RAPID module containing the RAPID symbol.
   * \param name for the name of the RAPID symbol.
   * \param p_data for storing the retrieved RAPID symbol data.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "correct parsing".
   */
  bool getRAPIDSymbolData(const std::string& task,
                          const std::string& module,
                          const std::string& name,
                          RAPIDSymbolDataAbstract* p_data);

  /**
   * \brief A method for retrieving the data of a RAPID symbol (parsed into a struct representing the RAPID data).
   *
   * \param task for the name of the RAPID task containing the RAPID symbol.
   * \param symbol indicating the RAPID symbol resource (name and module).
   * \param p_data for storing the retrieved RAPID symbol data.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "correct parsing".
   */
  bool getRAPIDSymbolData(const std::string& task,
                          const RWSClient::RAPIDSymbolResource& symbol,
                          RAPIDSymbolDataAbstract* p_data);

  /**
   * \brief A method for retrieving information about the RAPID modules of a RAPID task defined in the robot controller.
   *
   * \return std::vector<RAPIDModuleInfo> containing the RAPID modules information.
   */
  std::vector<RAPIDModuleInfo> getRAPIDModulesInfo(const std::string& task);

  /**
   * \brief A method for retrieving information about the RAPID tasks defined in the robot controller.
   *
   * \return std::vector<RAPIDTaskInfo> containing the RAPID tasks information.
   */
  std::vector<RAPIDTaskInfo> getRAPIDTasks();

  /**
   * \brief A method for retrieving some system information from the robot controller.
   *
   * \return SystemInfo containing the system information.
   */
  SystemInfo getSystemInfo();

  /**
   * \brief A method for checking if the robot controller mode is in auto mode.
   *
   * \return TriBool indicating if the mode is auto or not or unknown.
   */
  TriBool isAutoMode();

  /**
   * \brief A method for checking if the motors are on.
   *
   * \return TriBool indicating if the motors are on or not or unknown.
   */
  TriBool isMotorsOn();

  /**
   * \brief A method for checking if RAPID is running.
   *
   * \return TriBool indicating if RAPID is running or not or unknown.
   */
  TriBool isRAPIDRunning();

  /**
   * \brief A method for setting the value of an IO signal.
   *
   * \param iosignal for the name of the IO signal.
   * \param value for the IO signal's new value.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool setIOSignal(const std::string& iosignal, const std::string& value);

  /**
   * \brief A method for setting the data of a RAPID symbol.
   *
   * \param task for the name of the RAPID task containing the RAPID symbol.
   * \param module for the name of the RAPID module containing the RAPID symbol.
   * \param name for the name of the RAPID symbol.
   * \param data containing the RAPID symbol's new data.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool setRAPIDSymbolData(const std::string& task,
                          const std::string& module,
                          const std::string& name,
                          const RAPIDSymbolDataAbstract& data);

  /**
   * \brief A method for setting the data of a RAPID symbol.
   *
   * \param task for the name of the RAPID task containing the RAPID symbol.
   * \param symbol indicating the RAPID symbol resource (name and module).
   * \param data containing the RAPID symbol's new data.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool setRAPIDSymbolData(const std::string& task,
                          const RWSClient::RAPIDSymbolResource& symbol,
                          const RAPIDSymbolDataAbstract& data);

  /**
   * \brief A method for starting RAPID execution in the robot controller.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool startRAPIDExecution();

  /**
   * \brief A method for stopping RAPID execution in the robot controller.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool stopRAPIDExecution();

  /**
   * \brief A method for reseting the RAPID program pointer in the robot controller.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool resetRAPIDProgramPointer();

  /**
   * \brief A method for turning on the robot controller's motors.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool setMotorsOn();

  /**
   * \brief A method for turning off the robot controller's motors.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool setMotorsOff();

  /**
   * \brief A method for retrieving a file from the robot controller.
   *
   * Note: Depending on the file, then the content can be in text or binary format.
   *
   * \param resource specifying the file's directory and name.
   * \param p_file_content for containing the retrieved file content.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool getFile(const RWSClient::FileResource& resource, std::string* p_file_content);

  /**
   * \brief A method for uploading a file to the robot controller.
   *
   * \param resource specifying the file's directory and name.
   * \param file_content for the file's content.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool uploadFile(const RWSClient::FileResource& resource, const std::string& file_content);

  /**
   * \brief A method for deleting a file from the robot controller.
   *
   * \param resource specifying the file's directory and name.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool deleteFile(const RWSClient::FileResource& resource);

  /**
   * \brief A method for starting for a subscription.
   *
   * \param resources specifying the resources to subscribe to.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool startSubscription(const RWSClient::SubscriptionResources& resources);

  /**
   * \brief A method for waiting for a subscription event (use if the event content is irrelevant).
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool waitForSubscriptionEvent();

  /**
   * \brief A method for waiting for a subscription event (use if the event content is important).
   *
   * \param p_xml_document for storing the data received in the subscription event.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool waitForSubscriptionEvent(Poco::AutoPtr<Poco::XML::Document>* p_xml_document);

  /**
   * \brief A method for ending a active subscription.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool endSubscription();

  /**
   * \brief Force close the active subscription connection.
   *
   * This will cause waitForSubscriptionEvent() to return or throw.
   * It does not delete the subscription from the controller.
   *
   * The preferred way to close the subscription is to request the robot controller to end it via
   * endSubscription(). This function can be used to force the connection to close immediately in
   * case the robot controller is not responding.
   *
   * This function blocks until an active waitForSubscriptionEvent() has finished.
   */
  void forceCloseSubscription();

  /**
   * \brief A method for registering a user as local.
   *
   * \param username specifying the user name.
   * \param application specifying the external application.
   * \param location specifying the location.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool registerLocalUser(const std::string& username = SystemConstants::General::DEFAULT_USERNAME,
                         const std::string& application = SystemConstants::General::EXTERNAL_APPLICATION,
                         const std::string& location = SystemConstants::General::EXTERNAL_LOCATION);

  /**
   * \brief A method for registering a user as remote.
   *
   * \param username specifying the user name.
   * \param application specifying the external application.
   * \param location specifying the location.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool registerRemoteUser(const std::string& username = SystemConstants::General::DEFAULT_USERNAME,
                          const std::string& application = SystemConstants::General::EXTERNAL_APPLICATION,
                          const std::string& location = SystemConstants::General::EXTERNAL_LOCATION);

  /**
   * \brief A method for retrieving the internal log as a text string.
   *
   * \param verbose indicating if the log text should be verbose or not.
   *
   * \return std::string containing the log text. An empty text string is returned if the log is empty.
   */
  std::string getLogText(const bool verbose = false);

  /**
   * \brief Method for retrieving only the most recently logged event as a text string.
   *
   * \param verbose indicating if the log text should be verbose or not.
   *
   * \return std::string containing the log text. An empty text string is returned if the log is empty.
   */
  std::string getLogTextLatestEvent(const bool verbose = false);

  /**
   * \brief A method for setting the HTTP communication timeout.
   *
   * \param timeout for the HTTP communication timeout [microseconds].
   */
  void setHTTPTimeout(const Poco::Int64 timeout)
  {
    rws_client_.setHTTPTimeout(timeout);
  }

protected:
  /**
   * \brief A method for comparing a single text content (from a XML document node) with a specific string value.
   *
   * \param rws_result for containing a RWS communication result.
   * \param attribute for specifying the XML node's required attribute.
   * \param compare_string for specifying the comparison string.
   *
   * \return TriBool containing the result of the comparison.
   */
  TriBool compareSingleContent(const RWSClient::RWSResult& rws_result,
                               const XMLAttribute& attribute,
                               const std::string& compare_string);

  /**
   * \brief The RWS client used to communicate with the robot controller.
   */
  RWSClient rws_client_;
};

} // end namespace rws
} // end namespace abb

#endif
