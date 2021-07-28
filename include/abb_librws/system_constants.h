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

#ifndef RWS_SYSTEM_CONSTANTS_H
#define RWS_SYSTEM_CONSTANTS_H

#include <string>

#include "abb_librws_export.h"

namespace abb
{
namespace rws
{
/**
 * \brief Struct containing various constant values defined by default robot controller systems.
 */
struct SystemConstants
{
  /**
   * \brief Controller states related constants.
   */
  struct ABB_LIBRWS_EXPORT ContollerStates
  {
    /**
     * \brief Robot controller motor on.
     */
    static const std::string CONTROLLER_MOTOR_ON;

    /**
     * \brief Robot controller motor off.
     */
    static const std::string CONTROLLER_MOTOR_OFF;

    /**
     * \brief Robot controller auto mode.
     */
    static const std::string PANEL_OPERATION_MODE_AUTO;

    /**
     * \brief RAPID running.
     */
    static const std::string RAPID_EXECUTION_RUNNING;
  };

  /**
   * \brief General constants.
   */
  struct ABB_LIBRWS_EXPORT General
  {
    /**
     * \brief Default name of an application using RWS.
     */
    static const std::string EXTERNAL_APPLICATION;

    /**
     * \brief Default location of an application using RWS.
     */
    static const std::string EXTERNAL_LOCATION;

    /**
     * \brief Default port number for RWS communication.
     */
    static const unsigned short DEFAULT_PORT_NUMBER;

    /**
     * \brief Default password (for unconfigured robot controller systems).
     */
    static const std::string DEFAULT_PASSWORD;

    /**
     * \brief Default username (for unconfigured robot controller systems).
     */
    static const std::string DEFAULT_USERNAME;

    /**
     * \brief Local user.
     */
    static const std::string LOCAL;

    /**
     * \brief Mechanical unit name for ROB_1.
     */
    static const std::string MECHANICAL_UNIT_ROB_1;

    /**
     * \brief Mechanical unit name for ROB_2.
     */
    static const std::string MECHANICAL_UNIT_ROB_2;

    /**
     * \brief Mechanical unit name for ROB_3.
     */
    static const std::string MECHANICAL_UNIT_ROB_3;

    /**
     * \brief Mechanical unit name for ROB_4.
     */
    static const std::string MECHANICAL_UNIT_ROB_4;

    /**
     * \brief Mechanical unit name for ROB_L.
     */
    static const std::string MECHANICAL_UNIT_ROB_L;

    /**
     * \brief Mechanical unit name for ROB_R.
     */
    static const std::string MECHANICAL_UNIT_ROB_R;

    /**
     * \brief Remote user.
     */
    static const std::string REMOTE;

    /**
     * \brief Base coordinate system.
     */
    static const std::string COORDINATE_BASE;

    /**
     * \brief World coordinate system.
     */
    static const std::string COORDINATE_WORLD;

    /**
     * \brief Tool coordinate system.
     */
    static const std::string COORDINATE_TOOL;

    /**
     * \brief Work object (wobj) coordinate system.
     */
    static const std::string COORDINATE_WOBJ;
  };

  /**
   * \brief IO signal related constants.
   */
  struct ABB_LIBRWS_EXPORT IOSignals
  {
    /**
     * \brief Name of defined IO signal for smart gripper left position.
     *
     * Note: Requires the Smart Gripper product.
     */
    static const std::string HAND_ACTUAL_POSITION_L;

    /**
     * \brief Name of defined IO signal for smart gripper right position.
     *
     * Note: Requires the Smart Gripper product.
     */
    static const std::string HAND_ACTUAL_POSITION_R;

    /**
     * \brief Name of defined IO signal for smart gripper left speed.
     *
     * Note: Requires the Smart Gripper product.
     */
    static const std::string HAND_ACTUAL_SPEED_L;

    /**
     * \brief Name of defined IO signal for smart gripper right speed.
     *
     * Note: Requires the Smart Gripper product.
     */
    static const std::string HAND_ACTUAL_SPEED_R;

    /**
     * \brief Name of defined IO signal for smart gripper left calibration status.
     *
     * Note: Requires the Smart Gripper product.
     */
    static const std::string HAND_STATUS_CALIBRATED_L;

    /**
     * \brief Name of defined IO signal for smart gripper right calibration status.
     *
     * Note: Requires the Smart Gripper product.
     */
    static const std::string HAND_STATUS_CALIBRATED_R;

    /**
     * \brief High digital IO signal.
     */
    static const std::string HIGH;

    /**
     * \brief Low digital IO signal.
     */
    static const std::string LOW;
  };

  /**
   * \brief RAPID related constants.
   */
  struct ABB_LIBRWS_EXPORT RAPID
  {
    /**
     * \brief RAPID boolean false.
     */
    static const std::string RAPID_FALSE;

    /**
     * \brief RAPID boolean true.
     */
    static const std::string RAPID_TRUE;

    /**
     * \brief Default name for the first robot RAPID motion task.
     */
    static const std::string TASK_ROB_1;

    /**
     * \brief Default name for the second robot RAPID motion task.
     */
    static const std::string TASK_ROB_2;

    /**
     * \brief Default name for the third robot RAPID motion task.
     */
    static const std::string TASK_ROB_3;

    /**
     * \brief Default name for the fourth robot RAPID motion task.
     */
    static const std::string TASK_ROB_4;

    /**
     * \brief Default name for the IRB14000 (a.k.a YuMi) left arm robot RAPID motion task.
     */
    static const std::string TASK_ROB_L;

    /**
     * \brief Default name for the IRB14000 (a.k.a YuMi) right arm robot RAPID motion task.
     */
    static const std::string TASK_ROB_R;

    /**
     * \brief RAPID data type bool.
     */
    static const std::string TYPE_BOOL;

    /**
     * \brief RAPID data type dnum.
     */
    static const std::string TYPE_DNUM;

    /**
     * \brief RAPID data type num.
     */
    static const std::string TYPE_NUM;

    /**
     * \brief RAPID data type string.
     */
    static const std::string TYPE_STRING;
  };
};

} // end namespace rws
} // end namespace abb

#endif
