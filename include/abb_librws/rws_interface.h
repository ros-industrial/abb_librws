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

#include "rws_cfg.h"
#include "rws_client.h"
#include "rws_subscription.h"

#include <map>
#include <variant>
#include <chrono>


namespace abb
{
namespace rws
{

/**
* \brief Execution state of a RAPID task.
*/
enum class RAPIDTaskExecutionState
{
  UNKNOWN,      ///< The task state is unknown.
  READY,        ///< The task is ready.
  STOPPED,      ///< The task has been stopped.
  STARTED,      ///< The task has been started.
  UNINITIALIZED ///< The task has not been initialized.
};

/**
 * \brief Type of a mechanical unit.
 */
enum class MechanicalUnitType
{
  NONE,      ///< The unit has no type.
  TCP_ROBOT, ///< The unit is a TCP robot (has more than one joint, and can process commands in Cartesian space).
  ROBOT,     ///< The unit is a robot (has more than one joint, but can only process commands in joint space).
  SINGLE,    ///< The unit is a single (has only one joint).
  UNDEFINED  ///< The unit is undefined.
};

/**
 * \brief Mode of a mechanical unit.
 */
enum class MechanicalUnitMode
{
  UNKNOWN_MODE, ///< The unit mode is unknown.
  ACTIVATED,    ///< The unit has been activated.
  DEACTIVATED   ///< The unit has been deactivated.
};

/**
 * \brief A struct for containing static information of a mechanical unit.
 */
struct MechanicalUnitStaticInfo
{
  /**
   * \brief The unit's type.
   */
  MechanicalUnitType type;

  /**
   * \brief The RAPID task using the unit.
   */
  std::string task_name;

  /**
   * \brief Number of axes in the unit.
   */
  int axes;

  /**
   * \brief Total number of axes in the unit (including axes in possible integrated unit).
   */
  int axes_total;

  /**
   * \brief Name of another unit (that this unit is integrated into).
   *
   * Will be set to "NoIntegratedUnit" if this unit is not integrated into another unit.
   */
  std::string is_integrated_unit;

  /**
   * \brief Name of another unit (that is part of this unit).
   *
   * Will be set to "NoIntegratedUnit" if this unit has no integrated unit.
   */
  std::string has_integrated_unit;
};

/**
 * \brief A struct for containing dynamic information of a mechanical unit.
 */
struct MechanicalUnitDynamicInfo
{
  /**
   * \brief Name of the unit's active tool.
   */
  std::string tool_name;

  /**
   * \brief Name of the unit's active work object.
   */
  std::string wobj_name;

  /**
   * \brief Name of the unit's active payload.
   */
  std::string payload_name;

  /**
   * \brief Name of the unit's active total payload.
   */
  std::string total_payload_name;

  /**
   * \brief The unit's current state.
   */
  std::string status;

  /**
   * \brief The unit's current mode.
   */
  MechanicalUnitMode mode;

  /**
   * \brief The unit's current jogging mode.
   */
  std::string jog_mode;

  /**
   * \brief The unit's current coordinate system type.
   */
  Coordinate coord_system;
};

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

  /**
   * \brief The system's type (e.g. if it is a virtual controller system or not).
   */
  std::string system_type;

  /**
   * \brief The system's options.
   */
  std::vector<std::string> system_options;
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
   * \param is_active indicating if the task is active or not.
   * \param execution_state indicating the task's current execution state.
   */
  RAPIDTaskInfo(const std::string& name,
                const bool is_motion_task,
                const bool is_active,
                const RAPIDTaskExecutionState execution_state)
  :
  name(name),
  is_motion_task(is_motion_task),
  is_active(is_active),
  execution_state(execution_state)
  {}

  /**
   * \brief The task's name.
   */
  std::string name;

  /**
   * \brief Flag indicating if the task is a motion task.
   */
  bool is_motion_task;

  /**
   * \brief Flag indicating if the task is active or not.
   */
  bool is_active;

  /**
   * \brief The current execution state of the task.
   */
  RAPIDTaskExecutionState execution_state;
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
 * \brief Mapping from IO signal name to a value. 
 * 
 * The value of a digital signal is a \a bool, the value of an analog signal is a \a float.
 */
using IOSignalInfo = std::map<std::string, std::variant<bool, float>>;
  
  
/**
 * \brief A class for wrapping a Robot Web Services (RWS) client in a more user friendly interface.
 */
class RWSInterface
{
public:
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
   * \brief Retrieves the configuration instances for the arms defined in the system.
   *
   * \return std::vector<cfg::moc::Arm> containing a list of the arms defined in the system.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::moc::Arm> getCFGArms();

  /**
   * \brief Retrieves the configuration instances for the joints defined in the system.
   *
   * \return std::vector<cfg::moc::Joint> containing a list of the joints defined in the system.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::moc::Joint> getCFGJoints();

  /**
   * \brief Retrieves the configuration instances for the mechanical units defined in the system.
   *
   * \return std::vector<cfg::moc::MechanicalUnit> containing a list of the mechanical units defined in the system.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::moc::MechanicalUnit> getCFGMechanicalUnits();

  /**
   * \brief Retrieves the configuration instances for the mechanical unit groups defined in the system.
   *
   * \return std::vector<cfg::sys::MechanicalUnitGroup> containing a list of the mechanical unit groups in the system.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::sys::MechanicalUnitGroup> getCFGMechanicalUnitGroups();

  /**
   * \brief Retrieves the configuration instances for the present (RobotWare) options.
   *
   * \return std::vector<cfg::sys::PresentOption> containing a list of the present (RobotWare) options.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::sys::PresentOption> getCFGPresentOptions();

  /**
   * \brief Retrieves the configuration instances for the robots defined in the system.
   *
   * \return std::vector<cfg::moc::Robot> containing a list of robots defined in the system.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::moc::Robot> getCFGRobots();

  /**
   * \brief Retrieves the configuration instances for the singles defined in the system.
   *
   * \return std::vector<cfg::moc::Single> containing a list of the singles defined in the system.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::moc::Single> getCFGSingles();

  /**
   * \brief Retrieves the configuration instances for the transmissions defined in the system.
   *
   * \return std::vector<cfg::moc::Transmission> containing a list of the transmissions defined in the system.
   *
   * \throw std::runtime_error if failed to get or parse the configuration instances.
   */
  std::vector<cfg::moc::Transmission> getCFGTransmission();

  /**
   * \brief A method for retrieving the RobotWare options present on the active robot controller system.
   *
   * This method has been deprecated, please use 'getCFGPresentOptions()' instead.
   *
   * \return std::vector<RobotWareOptionInfo> containing a list of the present RobotWare options.
   */
  ABB_LIBRWS_DEPRECATED std::vector<RobotWareOptionInfo> getPresentRobotWareOptions();


  /// @brief Get value of a digital signal
  ///
  /// @param signal_name Name of the signal
  /// @return Value of the requested digital signal
  ///
  bool getDigitalSignal(std::string const& signal_name);


  /// @brief Get value of an analog signal
  ///
  /// @param signal_name Name of the signal
  /// @return Value of the requested analog signal
  ///
  float getAnalogSignal(std::string const& signal_name);


  /**
   * \brief Get values of all IO signals.
   * 
   * \return Mapping from IO signal names to values. 
   */
  IOSignalInfo getIOSignals();
  

  /**
   * \brief A method for retrieving static information about a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   * 
   * \return static information about a mechanical unit.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  MechanicalUnitStaticInfo getMechanicalUnitStaticInfo(const std::string& mechunit);

  /**
   * \brief A method for retrieving dynamic information about a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   * 
   * \return dynamic information about a mechanical unit.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  MechanicalUnitDynamicInfo getMechanicalUnitDynamicInfo(const std::string& mechunit);

  /**
   * \brief A method for retrieving the current jointtarget values of a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   * 
   * \return jointtarget data.
   *
   * \throw \a std::runtime_error if something goes wrong.
   */
  JointTarget getMechanicalUnitJointTarget(const std::string& mechunit);

  /**
   * \brief A method for retrieving the current robtarget values of a mechanical unit.
   *
   * \param mechunit for the mechanical unit's name.
   * \param coordinate for the coordinate mode (base, world, tool, or wobj) in which the robtarget will be reported.
   * \param tool for the tool frame relative to which the robtarget will be reported.
   * \param wobj for the work object (wobj) relative to which the robtarget will be reported.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   * 
   * \return robtarget data.
   */
  RobTarget getMechanicalUnitRobTarget(const std::string& mechunit,
                                  Coordinate coordinate = Coordinate::ACTIVE,
                                  const std::string& tool = "",
                                  const std::string& wobj = "");

  /**
   * \brief A method for retrieving the data of a RAPID symbol in raw text format.
   *
   * See the corresponding "setRAPIDSymbolData(...)" method for examples of RAPID symbols in raw text format.
   *
   * \param task name of the RAPID task containing the RAPID symbol.
   * \param module name of the RAPID module containing the RAPID symbol.
   * \param name name of the RAPID symbol.
   *
   * \return std::string containing the data. Empty if not found.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  std::string getRAPIDSymbolData(const std::string& task, const std::string& module, const std::string& name);


  /**
   * \brief Retrieves the data of a RAPID symbol (parsed into a struct representing the RAPID data).
   * 
   * \param resource specifies the RAPID task, module and symbol name.
   * \param data for storing the retrieved RAPID symbol data.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void getRAPIDSymbolData(RAPIDResource const& resource, RAPIDSymbolDataAbstract& data);


  /**
   * \brief A method for retrieving information about the RAPID modules of a RAPID task defined in the robot controller.
   *
   * \return \a std::vector<RAPIDModuleInfo> containing the RAPID modules information.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  std::vector<RAPIDModuleInfo> getRAPIDModulesInfo(const std::string& task);

  /**
   * \brief A method for retrieving information about the RAPID tasks defined in the robot controller.
   *
   * \return \a std::vector<RAPIDTaskInfo> containing the RAPID tasks information.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  std::vector<RAPIDTaskInfo> getRAPIDTasks();

  /**
   * \brief A method for retrieving the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
   *
   * \return unsigned int with the speed ratio in the range [0, 100] (ie: inclusive).
   *
   * \throw \a std::runtime_error if failed to get or parse the speed ratio.
   */
  unsigned int getSpeedRatio();

  /**
   * \brief A method for retrieving some system information from the robot controller.
   *
   * \return SystemInfo containing the system information (info will be empty if e.g. a timeout occurred).
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  SystemInfo getSystemInfo();

  /**
   * \brief A method for checking if the robot controller mode is in auto mode.
   *
   * \return if the mode is auto or not.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  bool isAutoMode();

  /**
   * \brief A method for checking if the motors are on.
   *
   * \return if the motors are on or not.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  bool isMotorsOn();

  /**
   * \brief A method for checking if RAPID is running.
   *
   * \return if RAPID is running or not.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  bool isRAPIDRunning();


  /// @brief Set value of a digital signal
  ///
  /// @param signal_name Name of the signal
  /// @param value New value of the signal
  ///
  void setDigitalSignal(std::string const& signal_name, bool value);


  /// @brief Set value of an analog signal
  ///
  /// @param signal_name Name of the signal
  /// @param value New value of the signal
  ///
  void setAnalogSignal(std::string const& signal_name, float value);


  /**
   * \brief A method for setting the data of a RAPID symbol via raw text format.
   *
   * Examples of RAPID symbols in raw text format:
   * - num: "1" or "-2.5".
   * - bool: "TRUE" or "FALSE".
   * - pos: "[1, -2, 3.3]".
   * - jointtarget: "[[1, -2, 3.3, -4.4, 5, 6], [9E9, 9E9, 9E9, 9E9, 9E9, 9E9]]"
   *
   * Notes:
   * - The absence of square brackets implies the symbol is of atomic data type.
   * - Record data types (composed of subcomponents) are always enclosed in square brackets.
   * - The value '9E9', in the jointtarget record, mean that the joint is not in use.
   *
   * Please see the "Technical reference manual - RAPID overview"
   * (document ID: 3HAC050947-001, revision: K) for more information
   * about basic RAPID data types and programming.
   *
   * \param task name of the RAPID task containing the RAPID symbol.
   * \param module name of the RAPID module containing the RAPID symbol.
   * \param name the name of the RAPID symbol.
   * \param data containing the RAPID symbol's new data.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void setRAPIDSymbolData(const std::string& task,
                          const std::string& module,
                          const std::string& name,
                          const std::string& data);


  /**
   * \brief A method for setting the data of a RAPID symbol.
   *
   * \param resource identifying the RAPID symbol.
   * \param data containing the RAPID symbol's new data.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void setRAPIDSymbolData(RAPIDResource const& resource,
                          const RAPIDSymbolDataAbstract& data);


  /**
   * \brief A method for starting RAPID execution in the robot controller.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void startRAPIDExecution();

  /**
   * \brief A method for stopping RAPID execution in the robot controller.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void stopRAPIDExecution();

  /**
   * \brief A method for reseting the RAPID program pointer in the robot controller.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void resetRAPIDProgramPointer();

  /**
   * \brief A method for turning on the robot controller's motors.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void setMotorsOn();

  /**
   * \brief A method for turning off the robot controller's motors.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void setMotorsOff();

  /**
   * \brief A method for setting the robot controller's speed ratio for RAPID motions (e.g. MoveJ and MoveL).
   *
   * Note: The ratio must be an integer in the range [0, 100] (ie: inclusive).
   *
   * \param ratio specifying the new ratio.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void setSpeedRatio(unsigned int ratio);

  /**
   * \brief A method for loading a module to the robot controller.
   *
   * \param task specifying the RAPID task.
   * \param resource specifying the file's directory and name.
   * \param replace indicating if the actual module into the controller must be replaced by the new one or not.
   *
   * \throw \a std::exception if something goes wrong.
   */
  void loadModuleIntoTask(const std::string& task, const FileResource& resource, const bool replace = false);

  /**
   * \brief A method for unloading a module to the robot controller.
   *
   * \param task specifying the RAPID task.
   * \param resource specifying the file's directory and name.
   *
   * \throw \a std::exception if something goes wrong.
   */
  void unloadModuleFromTask(const std::string& task, const FileResource& resource);

  /**
   * \brief A method for retrieving a file from the robot controller.
   *
   * Note: Depending on the file, then the content can be in text or binary format.
   *
   * \param resource specifying the file's directory and name.
   * 
   * \return file content.
   * 
   * \throw \a std::exception if something goes wrong.
   */
  std::string getFile(const FileResource& resource);

  /**
   * \brief A method for uploading a file to the robot controller.
   *
   * \param resource specifying the file's directory and name.
   * \param file_content for the file's content.
   * 
   * \throw \a std::exception if something goes wrong.
   */
  void uploadFile(const FileResource& resource, const std::string& file_content);

  /**
   * \brief A method for deleting a file from the robot controller.
   *
   * \param resource specifying the file's directory and name.
   * 
   * \throw \a std::exception if something goes wrong.
   */
  void deleteFile(const FileResource& resource);

  /**
   * \brief Creates a subscription group.
   *
   * \param resources specifying the resources to subscribe to.
   *
   * \return Newly created \a SubscriptionGroup for specified subscription resources.
   * 
   * \throw \a std::exception if something goes wrong
   */
  SubscriptionGroup openSubscription(const SubscriptionResources& resources);

  /**
   * \brief A method for registering a user as local.
   *
   * \param username specifying the user name.
   * \param application specifying the external application.
   * \param location specifying the location.
   * 
   * \throw \a std::exception if something goes wrong.
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
   * \throw \a std::exception if something goes wrong.
   */
  void registerRemoteUser(const std::string& username = SystemConstants::General::DEFAULT_USERNAME,
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
   * \param timeout HTTP communication timeout.
   */
  void setHTTPTimeout(std::chrono::microseconds timeout);

  /**
   * \brief Get HTTP communication timeout.
   * 
   * \return HTTP communication timeout.
   */
  std::chrono::microseconds getHTTPTimeout() const noexcept;

protected:
  /**
   * \brief A method for comparing a single text content (from a XML document node) with a specific string value.
   *
   * \param rws_result for containing a RWS communication result.
   * \param attribute for specifying the XML node's required attribute.
   * \param compare_string for specifying the comparison string.
   *
   * \return The result of the comparison.
   */
  static bool compareSingleContent(const RWSResult& rws_result,
                               const XMLAttribute& attribute,
                               const std::string& compare_string);

  /**
   * \brief The RWS client used to communicate with the robot controller.
   */
  RWSClient rws_client_;


private:
  /**
   * \brief A method for retrieving the value if an IO signal.
   *
   * \param iosignal for the name of the IO signal.
   *
   * \return std::string containing the IO signal's value (empty if not found).
   */
  std::string getIOSignal(const std::string& iosignal);


  /**
   * \brief A method for setting the value of an IO signal.
   *
   * \param iosignal for the name of the IO signal.
   * \param value for the IO signal's new value.
   * 
   * \throw \a std::runtime_error if something goes wrong.
   */
  void setIOSignal(const std::string& iosignal, const std::string& value);
};

} // end namespace rws
} // end namespace abb

#endif
