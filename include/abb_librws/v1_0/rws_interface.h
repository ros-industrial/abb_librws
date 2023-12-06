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

#include <abb_librws/v1_0/rws_client.h>
#include <abb_librws/v1_0/rws.h>
#include <abb_librws/v1_0/rw/rapid.h>
#include <abb_librws/v1_0/subscription.h>
#include <abb_librws/common/rw/io.h>
#include <abb_librws/rws.h>
#include <abb_librws/rws_cfg.h>
#include <abb_librws/rws_info.h>
#include <abb_librws/xml_attribute.h>

#include <chrono>
#include <cstdint>


namespace abb :: rws :: v1_0
{
/**
 * \brief User-friendly interface to Robot Web Services (RWS) 1.0.
 *
 * Only a subset of the features available in RWS are implemented here.
 * See https://developercenter.robotstudio.com/api/rwsApi/ for details about RWS 1.0
 */
class RWSInterface
{
public:
  /**
   * \brief A constructor.
   *
   * \param client RWS client.
   */
  explicit RWSInterface(RWSClient& client_);

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


  /// @brief Get value of a group signal
  ///
  /// @param signal_name Name of the signal
  /// @return Value of the requested group signal
  ///
  std::uint32_t getGroupSignal(std::string const& signal_name);


  /**
   * \brief Get values of all IO signals.
   *
   * \return Mapping from IO signal names to values.
   */
  rw::io::IOSignalInfo getIOSignals();


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
  std::vector<rw::RAPIDModuleInfo> getRAPIDModulesInfo(const std::string& task);

  /**
   * \brief A method for retrieving information about the RAPID tasks defined in the robot controller.
   *
   * \return \a std::vector<RAPIDTaskInfo> containing the RAPID tasks information.
   *
   * \throw \a std::runtime_error if something goes wrong.
   */
  std::vector<rw::RAPIDTaskInfo> getRAPIDTasks();

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


  /// @brief Set value of a group signal
  ///
  /// @param signal_name Name of the signal
  /// @param value New value of the signal
  ///
  void setGroupSignal(std::string const& signal_name, std::uint32_t value);


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
   * There can be a delay between the function returns and when the RAPID program enters the "running" state.
   *
   * \throw \a std::runtime_error if something goes wrong.
   */
  void startRAPIDExecution();

  /**
   * \brief A method for stopping RAPID execution in the robot controller.
   *
   * https://developercenter.robotstudio.com/api/rwsApi/rapid_execution_stop_page.html
   *
   * There can be a delay between the function returns and when the RAPID program enters the "stopped" state.
   *
   * \param stopmode stop mode
   * \param usetsp which tasks to stop (?)
   *
   * \throw \a std::runtime_error if something goes wrong.
   */
  void stopRAPIDExecution(StopMode stopmode = StopMode::stop, UseTsp usetsp = UseTsp::normal);

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
   * @brief Save the config domain to the given file.
   * @param resource File on the controller to save to.
   * @param domain Paramters domain to save.
   */
  void saveConfigDomain(FileResource const& resource, abb::rws::CFGDomain const& domain);

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
  std::unique_ptr<subscription::SubscriptionGroup> openSubscription(const SubscriptionResources& resources);

  /**
   * \brief A method for registering a user as local.
   *
   * \param username specifying the user name.
   * \param application specifying the external application.
   * \param location specifying the location.
   *
   * \throw \a std::exception if something goes wrong.
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
   * \throw \a std::exception if something goes wrong.
   */
  void registerRemoteUser(const std::string& username = DEFAULT_USERNAME,
                          const std::string& application = SystemConstants::General::EXTERNAL_APPLICATION,
                          const std::string& location = SystemConstants::General::EXTERNAL_LOCATION);

  /**
   * \brief Request mastership of all domains.
   */
  void requestMastership();

  /**
   * \brief Release mastership of all domains.
   */
  void releaseMastership();

  /**
   * \brief Request mastership of a given domain.
   *
   * \param domain domain for which mastership is requested. If empty, mastership for all domains will be requested.
   */
  void requestMastership(MastershipDomain domain);


  /**
   * \brief Release mastership of a given domain.
   *
   * \param domain domain for which mastership is to be requested. If empty, mastership for all domains will be released.
   */
  void releaseMastership(MastershipDomain domain);

  /**
   * \brief Activate all tasks.
   */
  void activateAllTasks();

  /**
   * \brief Deactivate all tasks.
   */
  void deactivateAllTasks();

private:
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
   * \brief A method for retrieving the value of an IO signal.
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


  /**
   * \brief The RWS client used to communicate with the robot controller.
   */
  RWSClient& rws_client_;
};

} // end namespace rws
