#pragma once

#include <iosfwd>


namespace abb :: rws
{
  /// @brief Modes for stopping RAPID execution.
  ///
  /// https://developercenter.robotstudio.com/api/rwsApi/rapid_execution_stop_page.html
  ///
  enum class StopMode
  {
    cycle,
    instr,
    stop,
    qstop
  };


  std::ostream& operator<<(std::ostream& os, StopMode stopmode);


  /// @brief (?) Defines which tasks should be stopped.
  ///
  /// https://developercenter.robotstudio.com/api/rwsApi/rapid_execution_stop_page.html
  ///
  enum class UseTsp
  {
    normal,
    alltsk
  };


  std::ostream& operator<<(std::ostream& os, UseTsp usetsp);


  /// @brief RWS mastership domains.
  ///
  enum class MastershipDomain
  {
    cfg,
    motion,
    rapid
  };


  std::ostream& operator<<(std::ostream& os, MastershipDomain domain);
}