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

#include <abb_librws/rws_common.h>


namespace abb
{
namespace rws
{
/***********************************************************************************************************************
 * Struct definitions: SystemConstants
 */

const std::string SystemConstants::ContollerStates::CONTROLLER_MOTOR_ON       = "motoron";
const std::string SystemConstants::ContollerStates::CONTROLLER_MOTOR_OFF      = "motoroff";
const std::string SystemConstants::ContollerStates::PANEL_OPERATION_MODE_AUTO = "AUTO";
const std::string SystemConstants::ContollerStates::RAPID_EXECUTION_RUNNING   = "running";

const std::string SystemConstants::General::EXTERNAL_APPLICATION   = "ExternalApplication";
const std::string SystemConstants::General::EXTERNAL_LOCATION      = "ExternalLocation";
const unsigned short SystemConstants::General::DEFAULT_PORT_NUMBER = 80;
const std::string SystemConstants::General::DEFAULT_PASSWORD       = "robotics";
const std::string SystemConstants::General::DEFAULT_USERNAME       = "Default User";
const std::string SystemConstants::General::LOCAL                  = "local";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_1  = "ROB_1";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_2  = "ROB_2";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_3  = "ROB_3";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_4  = "ROB_4";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_L  = "ROB_L";
const std::string SystemConstants::General::MECHANICAL_UNIT_ROB_R  = "ROB_R";
const std::string SystemConstants::General::REMOTE                 = "remote";
const std::string SystemConstants::General::COORDINATE_BASE        = "Base";
const std::string SystemConstants::General::COORDINATE_WORLD       = "Word";
const std::string SystemConstants::General::COORDINATE_TOOL        = "Tool";
const std::string SystemConstants::General::COORDINATE_WOBJ        = "Wobj";

const std::string SystemConstants::IOSignals::HAND_ACTUAL_POSITION_L   = "hand_ActualPosition_L";
const std::string SystemConstants::IOSignals::HAND_ACTUAL_POSITION_R   = "hand_ActualPosition_R";
const std::string SystemConstants::IOSignals::HAND_ACTUAL_SPEED_L      = "hand_ActualSpeed_L";
const std::string SystemConstants::IOSignals::HAND_ACTUAL_SPEED_R      = "hand_ActualSpeed_R";
const std::string SystemConstants::IOSignals::HAND_STATUS_CALIBRATED_L = "hand_StatusCalibrated_L";
const std::string SystemConstants::IOSignals::HAND_STATUS_CALIBRATED_R = "hand_StatusCalibrated_R";
const std::string SystemConstants::IOSignals::HIGH                     = "1";
const std::string SystemConstants::IOSignals::LOW                      = "0";

const std::string SystemConstants::RAPID::RAPID_FALSE = "FALSE";
const std::string SystemConstants::RAPID::RAPID_TRUE  = "TRUE";
const std::string SystemConstants::RAPID::TASK_ROB_1  = "T_ROB1";
const std::string SystemConstants::RAPID::TASK_ROB_2  = "T_ROB2";
const std::string SystemConstants::RAPID::TASK_ROB_3  = "T_ROB3";
const std::string SystemConstants::RAPID::TASK_ROB_4  = "T_ROB4";
const std::string SystemConstants::RAPID::TASK_ROB_L  = "T_ROB_L";
const std::string SystemConstants::RAPID::TASK_ROB_R  = "T_ROB_R";
const std::string SystemConstants::RAPID::TYPE_BOOL   = "bool";
const std::string SystemConstants::RAPID::TYPE_DNUM   = "dnum";
const std::string SystemConstants::RAPID::TYPE_NUM    = "num";
const std::string SystemConstants::RAPID::TYPE_STRING = "string";

} // end namespace rws
} // end namespace abb
