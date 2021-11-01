#pragma once

#include "coordinate.h"

#include <abb_librws/common/rw/rapid.h>

#include <string>
#include <vector>
#include <map>
#include <variant>


namespace abb :: rws
{
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
   * \brief A struct for containing static information (at least during runtime) about the robot controller.
   */
  struct StaticInfo
  {
    /**
     * \brief Information about the defined RAPID tasks.
     */
    std::vector<rw::RAPIDTaskInfo> rapid_tasks;

    /**
     * \brief System information.
     */
    SystemInfo system_info;
  };


  /**
   * \brief Mapping from IO signal name to a value.
   *
   * The value of a digital signal is a \a bool, the value of an analog signal is a \a float.
   */
  using IOSignalInfo = std::map<std::string, std::variant<bool, float>>;
}