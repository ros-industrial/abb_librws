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

#ifndef RWS_SIMPLE_STATE_MACHINE_INTERFACE_H
#define RWS_SIMPLE_STATE_MACHINE_INTERFACE_H

#include "rws_interface.h"

namespace abb
{
namespace rws
{
/**
 * \brief A class for a user friendly interface to a corresponding RAPID program.
 *
 * Note: This class assumes that certain corresponding RAPID symbols and IO signals and other
 *       configurations has been defined on the robot controller side.
 *
 *       I.e. the robot controller needs to be properly configured.
 */
class RWSSimpleStateMachineInterface : public RWSInterface
{
public:
  /**
   * \brief Struct for representing a custom RAPID record for arguments to EGMSetupUC instructions.
   *
   * Note: Assumed to have been defined in the RAPID code.
   */
  struct EGMSetupUCArgs : public RAPIDRecord
  {
    EGMSetupUCArgs()
    :
    RAPIDRecord("EGMSetupUCArgs")
    {
      components_.push_back(&use_filtering);
      components_.push_back(&comm_timeout);
    }
  
    /**
     * \brief Flag indicating if the EGM controller should apply extra filtering on the EGM corrections.
     *
     * Note: If true: Applies extra filtering on the correction, but also introduces some extra delays and latency.
     *       Else: Raw corrections will be used. Low-pass filtering is then necessary to avoid vibrations in the robot.
     */
    RAPIDBool use_filtering;
  
    /**
     * \brief EGM communication timeout [s].
     */
    RAPIDNum comm_timeout;
  };

  /**
   * \brief Struct for representing a custom RAPID record for arguments to EGMAct instructions.
   *
   * Note: Assumed to have been defined in the RAPID code.
   */
  struct EGMActivateArgs : public RAPIDRecord
  {
  public:
    /**
     * \brief A default constructor.
     */
    EGMActivateArgs()
    :
    RAPIDRecord("EGMActivateArgs")
    {
      components_.push_back(&tool);
      components_.push_back(&wobj);
      components_.push_back(&cond_min_max);
      components_.push_back(&lp_filter);
      components_.push_back(&sample_rate);
      components_.push_back(&max_speed_deviation);
    };
    
    /**
     * \brief The tool to use.
     */
    ToolData tool;
    
    /**
     * \brief The work object to use.
     */
    WObjData wobj;

    /**
     * \brief Condition value [deg or mm] for when the EGM correction is considered to be finished.
     * 
     * E.g. for joint mode, then the condition is fulfilled when the joints are within [-cond_min_max, cond_min_max].
     */
    RAPIDNum cond_min_max;
    
    /**
     * \brief Low pass filer bandwidth for the EGM controller [Hz].
     */
    RAPIDNum lp_filter;
    
    /**
     * \brief Sample rate for the EGM communication [ms]. 
     *
     * Only multiples of 4 are allowed. I.e. 4, 8, 16, etc.
     */
    RAPIDNum sample_rate;

    /**
     * \brief Maximum admitted joint speed change [deg/s]:
     * 
     * Note: Take care if setting this higher than the lowest max speed [deg/s],
     *       out of all the axis max speeds (found in the robot's data sheet).
     */
    RAPIDNum max_speed_deviation;
  };

  /**
   * \brief Struct for representing a custom RAPID record for arguments to EGMRun instructions.
   *
   * Note: Assumed to have been defined in the RAPID code.
   */
  struct EGMRunArgs : public RAPIDRecord
  {
    /**
     * \brief A default constructor.
     */
    EGMRunArgs()
    :
    RAPIDRecord("EGMRunArgs")
    {
      components_.push_back(&cond_time);
      components_.push_back(&ramp_in_time);
      components_.push_back(&pos_corr_gain);
    }
  
    /**
     * \brief Condition time for EGM motion [s].
     */
    RAPIDNum cond_time;
    
    /**
     * \brief Ramp in time for EGM motion [s].
     */
    RAPIDNum ramp_in_time;
    
    /**
     * \brief Position correction gain for EGM motion.
     */
    RAPIDNum pos_corr_gain;
  };

  /**
   * \brief Struct for representing a custom RAPID record for arguments to EGMStop instructions.
   *
   * Note: Assumed to have been defined in the RAPID code.
   */
  struct EGMStopArgs : public RAPIDRecord
  {
    /**
     * \brief A default constructor.
     */
    EGMStopArgs()
    :
    RAPIDRecord("EGMStopArgs")
    {
      components_.push_back(&ramp_out_time);
    }
  
    /**
     * \brief Desired duration for ramping out EGM motions [s].
     */
    RAPIDNum ramp_out_time;
  };

  /**
   * \brief Struct for representing a custom RAPID record for runtime arguments to EGM instructions.
   *
   * Note: Assumed to have been defined in the RAPID code.
   */
  struct EGMRuntimeArgs : public RAPIDRecord
  {
    /**
     * \brief A default constructor.
     */
    EGMRuntimeArgs()
    :
    RAPIDRecord("EGMRuntimeArgs")
    {
      components_.push_back(&setup_uc);
      components_.push_back(&activate);
      components_.push_back(&run);
      components_.push_back(&stop);
    }
  
    /**
     * \brief Arguments for EGMSetupUC instruction.
     */
    EGMSetupUCArgs setup_uc;
    
    /**
     * \brief Arguments for EGMAct instructions.
     */
    EGMActivateArgs activate;
    
    /**
     * \brief Arguments for EGMRun instructions.
     */
    EGMRunArgs run;
  
    /**
     * \brief Arguments for EGMStop instruction.
     */
    EGMStopArgs stop;
  };

  /**
   * \brief Struct containing various program-specific constant values.
   *
   * Note: Assumed to have been defined in robot controller.
   */
  struct ProgramConstants
  {
    /**
     * \brief Struct for program-specific IO signals.
     */
    struct IOSignals
    {
      /**
       * \brief Name of defined IO signal for starting EGM joint mode.
       *
       * Note: Assumed to have been setup in the robot controller.
       */
      static const std::string EGM_START_JOINT;
        
      /**
       * \brief Name of defined IO signal for starting EGM pose mode.
       *
       * Note: Assumed to have been setup in the robot controller.
       */
      static const std::string EGM_START_POSE;
        
      /**
       * \brief Name of defined IO signal for EGM stop.
       *
       * Note: Assumed to have been setup in the robot controller.
       */
      static const std::string EGM_STOP;
        
      /**
       * \brief Prefix for defined IO signal(s), for checking if a mechanical unit is stationary.
       *
       * Note: Assumed to have been setup in the robot controller, as well as 
       *       connected to the system output for "Mechanical Unit Not Moving".
       */
      static const std::string OUTPUT_STATIONARY;

      /**
       * \brief Name of defined IO signal for running RAPID routines.
       *
       * Note: Assumed to have been setup in the robot controller.
       */
      static const std::string RUN_RAPID_ROUTINE;
    };
    
    /**
     * \brief Struct for program-specific RAPID identifiers.
     */
    struct RAPID
    {
      /**
       * \brief Struct for program-specific RAPID modules.
       */
      struct Modules
      {
        /**
         * \brief Name of a special EGM module.
         *
         * Note: Assumed to have been loaded into a RAPID motion task.
         */
        static const std::string T_ROB_EGM;
        
        /**
         * \brief Name of a special EGM presynching module.
         *
         * Note: Assumed to have been loaded into a RAPID motion task.
         */
        static const std::string T_ROB_EGM_PRESYNCHING;

        /**
         * \brief Name of a special main module.
         *
         * Note: Assumed to have been loaded into a RAPID motion task.
         */
        static const std::string T_ROB_MAIN;

        /**
         * \brief Name of a special RAPID module.
         *
         * Note: Assumed to have been loaded into a RAPID motion task.
         */
        static const std::string T_ROB_RAPID;

        /**
         * \brief Name of a special utility module.
         *
         * Note: Assumed to have been loaded into a RAPID motion task.
         */
        static const std::string T_ROB_UTILITY;
      };
      
      /**
       * \brief Struct for program-specific RAPID symbols.
       */
      struct Symbols
      {
        /**
         * \brief A defined RAPID symbol for setting if EGM is allowed or not.
         *
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const RWSClient::RAPIDSymbolResource ALLOW_EGM;

        /**
         * \brief A defined RAPID symbol for the current jointtarget.
         *
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const RWSClient::RAPIDSymbolResource CURRENT_JOINTTARGET;
        
        /**
         * \brief A defined RAPID symbol for current robtarget.
         *
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const RWSClient::RAPIDSymbolResource CURRENT_ROBTARGET;

        /**
         * \brief A defined RAPID symbol for current state.
         *
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const RWSClient::RAPIDSymbolResource CURRENT_STATE;

        /**
         * \brief A defined RAPID symbol for EGM runtime arguments.
         *
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const RWSClient::RAPIDSymbolResource EGM_RUNTIME;
        
        /**
         * \brief A defined RAPID symbol for setting if EGM presynching should be used or not.
         *
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const RWSClient::RAPIDSymbolResource USE_EGM_PRESYNC;

        /**
         * \brief A defined RAPID symbol for a RAPID routine name.
         *
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const RWSClient::RAPIDSymbolResource ROUTINE_NAME;
      };
      
      /**
       * \brief Struct for program-specific RAPID symbol values.
       */
      struct Values
      {
        /**
         * \brief Value of a defined RAPID value for an idle state.
         * 
         * Note: Assumed to have been declared in a RAPID module.
         */
        static const float STATE_IDLE;
      };
    };
  };
  
  /**
   * \brief A constructor.
   *
   * \param ip_address specifying the robot controller's IP address.
   */
  RWSSimpleStateMachineInterface(const std::string ip_address)
  :
  RWSInterface(ip_address,
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
  RWSSimpleStateMachineInterface(const std::string ip_address, const std::string username, const std::string password)
  :
  RWSInterface(ip_address,
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
  RWSSimpleStateMachineInterface(const std::string ip_address, const unsigned short port)
  :
  RWSInterface(ip_address,
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
  RWSSimpleStateMachineInterface(const std::string ip_address,
                                 const unsigned short port,
                                 const std::string username,
                                 const std::string password)
  :
  RWSInterface(ip_address,
               port,
               username,
               password)
  {}

  /**
   * \brief A method for retrieving the current jointtarget values of the robot.
   *
   * \param task for the name of the RAPID task containing the symbol.
   * \param p_current_jointtarget for storing the retrieved data.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "correct parsing".
   */
  bool getCurrentJointTarget(const std::string task, JointTarget* p_current_jointtarget);
  
  /**
   * \brief A method for retrieving the current robtarget values of the robot.
   *
   * \param task for the name of the RAPID task containing the symbol.
   * \param p_current_robtarget for storing the retrieved data.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "correct parsing".
   */
  bool getCurrentRobTarget(const std::string task, RobTarget* p_current_robtarget);

  /**
   * \brief A method for retrieving the data of a RAPID symbol (of the RECORD type EGMRuntimeArgs).
   *
   * \param task for the name of the RAPID task containing the symbol.
   * \param p_egm_runtime_arguments for storing the retrieved data.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "correct parsing".
   */
  bool getEGMRecord(const std::string task, EGMRuntimeArgs* p_egm_runtime_arguments);
  
  /**
   * \brief A method for going to the calibration position.
   *
   * \param task for the name of the RAPID task containing the symbol.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "action started".
   */
  bool goToCalibrationPosition(const std::string task);
  
  /**
   * \brief A method for going to the home position.
   *
   * \param task for the name of the RAPID task containing the symbol.
   *
   * \return bool indicating if the communication was successful or not. Note: No checks are made for "action started".
   */
  bool goToHomePosition(const std::string task);

  /**
   * \brief A method for checking if the RAPID task's state machine program is in idle state.
   *
   * Note: It is assumed that the "SimpleStateMachine" RAPID program is used (or a RAPID program based on it at least).
   *
   * \param task for the name of the RAPID task to check.
   *
   * \return TriBool indicating if the state is idle or not or unknown.
   */
  TriBool isStateIdle(const std::string task);
  
  /**
   * \brief A method for checking if a mechanical unit is stationary.
   *
   * Note: It is assumed that the robot controller has been configured accordingly.
   *
   * \param mechanical_unit for the name of the mechanical unit to check.
   *
   * \return TriBool indicating if the mechanical unit is stationary or not or unknown.
   */
  TriBool isStationary(const std::string mechanical_unit);

  /**
   * \brief A method for setting the data of a RAPID symbol (of the RECORD type EGM_RECORD).
   *
   * \param task for the name of the RAPID task containing the RAPID symbol.
   * \param egm_runtime_arguments containing the symbol's new data.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool setEGMRecord(const std::string task, EGMRuntimeArgs egm_runtime_arguments);
  
  /**
   * \brief Signal that the controller should start an EGM joint motion.
   *
   * \return bool indicating if the signaling was successful.
   */
  bool signalEGMStartJoint();
  
  /**
   * \brief Signal that the controller should start an EGM pose motion.
   *
   * \return bool indicating if the signaling was successful.
   */
  bool signalEGMStartPose();
  
  /**
   * \brief Signal that the controller should stop any current EGM motions.
   *
   * \return bool indicating if the signaling was successful.
   */
  bool signalEGMStop();
  
  /**
   * \brief Signal that the controller should run a prenamed RAPID routine for the specified task.
   *
   * \param task for the name of the RAPID task containing the RAPID routine.
   * \param routine_name for the name of the routine to run.
   *
   * \return bool indicating if the communication was successful or not.
   */
  bool signalRunRAPIDRoutine(const std::string task, const std::string routine_name);

private:
  /**
   * \brief Toggles an IO signal.
   *
   * \param iosignal specifying the name of the IO signal to toggle.
   *
   * \return bool indicating if the toggling was successful.
   */
  bool toggleIOSignal(const std::string iosignal);
};

} // end namespace rws
} // end namespace abb

#endif