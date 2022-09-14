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

#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/v2_0/rws.h>
#include <abb_librws/v2_0/subscription.h>

#include <abb_librws/rws_cfg.h>
#include <abb_librws/common/rw/io.h>
#include <abb_librws/rws_info.h>
#include <abb_librws/xml_attribute.h>

#include <chrono>
#include <cstdint>


namespace abb :: rws :: v2_0
{
/**
 * \brief User-friendly interface to Robot Web Services (RWS) 2.0.
 *
 * Only a subset of the features available in RWS are implemented here.
 * See https://developercenter.robotstudio.com/api/RWS for details about RWS 2.0
 */
class RWSInterface
{
public:
  /**
   * \brief A constructor.
   *
   * \param client RWS client
   */
  explicit RWSInterface(RWSClient& client);

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
   * \brief A method for retrieving some system information from the robot controller.
   *
   * \return SystemInfo containing the system information (info will be empty if e.g. a timeout occurred).
   *
   * \throw \a std::runtime_error if something goes wrong.
   */
  SystemInfo getSystemInfo();

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
   * \brief Request RWS mastership
   *
   * \param domain requested mastership domain
   */
  void requestMastership(MastershipDomain domain);

  /**
   * \brief Release RWS mastership
   *
   * \param domain mastership domain to be released
   */
  void releaseMastership(MastershipDomain domain);

private:
  using RWSResult = RWSClient::RWSResult;


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


  /**
   * \brief The RWS client used to communicate with the robot controller.
   */
  RWSClient& rws_client_;
};

} // end namespace rws
