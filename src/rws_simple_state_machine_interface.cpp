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

#include "abb_librws/rws_simple_state_machine_interface.h"

namespace abb
{
namespace rws
{
/***********************************************************************************************************************
 * Struct definitions: RWSSimpleStateMachineInterface::ProgramConstants
 */

typedef RWSSimpleStateMachineInterface::ProgramConstants::IOSignals      IOSignals;
typedef RWSSimpleStateMachineInterface::ProgramConstants::RAPID::Modules Modules;
typedef RWSSimpleStateMachineInterface::ProgramConstants::RAPID::Symbols Symbols;
typedef RWSSimpleStateMachineInterface::ProgramConstants::RAPID::Values  Values;

const std::string IOSignals::EGM_START_JOINT   = "EGM_START_JOINT";
const std::string IOSignals::EGM_START_POSE    = "EGM_START_POSE";
const std::string IOSignals::EGM_STOP          = "EGM_STOP";
const std::string IOSignals::OUTPUT_STATIONARY = "OUTPUT_STATIONARY";
const std::string IOSignals::RUN_RAPID_ROUTINE = "RUN_RAPID_ROUTINE";
  
const std::string Modules::T_ROB_EGM             = "TRobEGM";
const std::string Modules::T_ROB_EGM_PRESYNCHING = "TRobEGMPresynching";
const std::string Modules::T_ROB_MAIN            = "TRobMain";
const std::string Modules::T_ROB_RAPID           = "TRobRAPID";
const std::string Modules::T_ROB_UTILITY         = "TRobUtility";

const RWSClient::RAPIDSymbolResource Symbols::ALLOW_EGM(Modules::T_ROB_EGM,                   "allow_egm");
const RWSClient::RAPIDSymbolResource Symbols::CURRENT_JOINTTARGET(Modules::T_ROB_UTILITY,     "current_jointtarget");
const RWSClient::RAPIDSymbolResource Symbols::CURRENT_ROBTARGET(Modules::T_ROB_UTILITY,       "current_robtarget");
const RWSClient::RAPIDSymbolResource Symbols::CURRENT_STATE(Modules::T_ROB_MAIN,              "current_state");
const RWSClient::RAPIDSymbolResource Symbols::EGM_RUNTIME(Modules::T_ROB_EGM,                 "egm_runtime");
const RWSClient::RAPIDSymbolResource Symbols::USE_EGM_PRESYNC(Modules::T_ROB_EGM_PRESYNCHING, "use_egm_presync");
const RWSClient::RAPIDSymbolResource Symbols::ROUTINE_NAME(Modules::T_ROB_RAPID,              "routine_name");

const float Values::STATE_IDLE = (float) 0.0;

 


/***********************************************************************************************************************
 * Class definitions: RWSSimpleStateMachineInterface
 */

/************************************************************
 * Primary methods
 */

bool RWSSimpleStateMachineInterface::getCurrentJointTarget(const std::string task, JointTarget* p_current_jointtarget)
{
  return getRAPIDSymbolData(task, ProgramConstants::RAPID::Symbols::CURRENT_JOINTTARGET, p_current_jointtarget);
}

bool RWSSimpleStateMachineInterface::getCurrentRobTarget(const std::string task, RobTarget* p_current_robtarget)
{
  return getRAPIDSymbolData(task, ProgramConstants::RAPID::Symbols::CURRENT_ROBTARGET, p_current_robtarget);
}

bool RWSSimpleStateMachineInterface::getEGMRecord(const std::string task, EGMRuntimeArgs* p_egm_runtime_arguments)
{
  return getRAPIDSymbolData(task, ProgramConstants::RAPID::Symbols::EGM_RUNTIME, p_egm_runtime_arguments);
}

bool RWSSimpleStateMachineInterface::goToCalibrationPosition(const std::string task)
{
  return signalRunRAPIDRoutine(task, "goToCalibrationPosition");
}

bool RWSSimpleStateMachineInterface::goToHomePosition(const std::string task)
{
  return signalRunRAPIDRoutine(task, "goToHomePosition");
}

TriBool RWSSimpleStateMachineInterface::isStateIdle(const std::string task)
{
  TriBool result;

  RAPIDAtomic<RAPID_NUM> current_state;
  if(getRAPIDSymbolData(task, ProgramConstants::RAPID::Symbols::CURRENT_STATE, &current_state))
  {
    result = (current_state.value == ProgramConstants::RAPID::Values::STATE_IDLE);
  }

  return result;
}

TriBool RWSSimpleStateMachineInterface::isStationary(const std::string mechanical_unit)
{
  TriBool result;

  std::string temp = getIOSignal(ProgramConstants::IOSignals::OUTPUT_STATIONARY + "_" + mechanical_unit);

  if (!temp.empty())
  {
    result = (temp == SystemConstants::IOSignals::HIGH);
  }

  return result;
}

bool RWSSimpleStateMachineInterface::setEGMRecord(const std::string task, EGMRuntimeArgs egm_runtime_arguments)
{
  return setRAPIDSymbolData(task, ProgramConstants::RAPID::Symbols::EGM_RUNTIME, egm_runtime_arguments);
}

bool RWSSimpleStateMachineInterface::signalEGMStartJoint()
{
  return toggleIOSignal(ProgramConstants::IOSignals::EGM_START_JOINT);
}

bool RWSSimpleStateMachineInterface::signalEGMStartPose()
{
  return toggleIOSignal(ProgramConstants::IOSignals::EGM_START_POSE);
}

bool RWSSimpleStateMachineInterface::signalEGMStop()
{
  return toggleIOSignal(ProgramConstants::IOSignals::EGM_STOP);
}

bool RWSSimpleStateMachineInterface::signalRunRAPIDRoutine(const std::string task, const std::string routine_name)
{
  RAPIDAtomic<RAPID_STRING> temp;
  temp.value = routine_name;

  bool result = setRAPIDSymbolData(task, ProgramConstants::RAPID::Symbols::ROUTINE_NAME, temp);

  if (result)
  {
    result = toggleIOSignal(ProgramConstants::IOSignals::RUN_RAPID_ROUTINE);
  }

  return result;
}

/************************************************************
 * Auxiliary methods
 */

bool RWSSimpleStateMachineInterface::toggleIOSignal(const std::string iosignal)
{
  bool result = false;
  int max_number_of_attempts = 5;

  if (isAutoMode().isTrue())
  {
    for (int i = 0; i < max_number_of_attempts && !result; ++i)
    {
      result = setIOSignal(iosignal, SystemConstants::IOSignals::LOW);
      if (result)
      {
        result = (getIOSignal(iosignal) == SystemConstants::IOSignals::LOW);
      }
    }

    if (result)
    {
      result = false;

      for (int i = 0; i < max_number_of_attempts && !result; ++i)
      {
        result = setIOSignal(iosignal, SystemConstants::IOSignals::HIGH);
        if (result)
        {
          result = (getIOSignal(iosignal) == SystemConstants::IOSignals::HIGH);
        }
      }
    }
  }

  return result;
}

} // end namespace rws
} // end namespace abb