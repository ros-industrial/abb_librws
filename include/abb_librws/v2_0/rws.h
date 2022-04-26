#pragma once

#include <abb_librws/xml_attribute.h>
#include <abb_librws/abb_librws_export.h>

#include <iosfwd>


namespace abb :: rws :: v2_0
{
  /**
   * \brief XML attributes specifying names with corresponding values.
   */
  struct ABB_LIBRWS_EXPORT XMLAttributes
  {
    /**
     * \brief Class & active type.
     */
    static const XMLAttribute CLASS_ACTIVE;

    /**
     * \brief Class & cfg-dt-instance-li.
     */
    static const XMLAttribute CLASS_CFG_DT_INSTANCE_LI;

    /**
     * \brief Class & cfg-ia-t-li.
     */
    static const XMLAttribute CLASS_CFG_IA_T_LI;

    /**
     * \brief Class & controller type.
     */
    static const XMLAttribute CLASS_CTRL_TYPE;

    /**
     * \brief Class & controller execution state.
     */
    static const XMLAttribute CLASS_CTRLEXECSTATE;

    /**
     * \brief Class & controller state.
     */
    static const XMLAttribute CLASS_CTRLSTATE;

    /**
     * \brief Class & data type.
     */
    static const XMLAttribute CLASS_DATTYP;

    /**
     * \brief Class & excstate type.
     */
    static const XMLAttribute CLASS_EXCSTATE;

    /**
     * \brief Class & ios-signal.
     */
    static const XMLAttribute CLASS_IOS_SIGNAL;

    /**
     * \brief Class & lvalue.
     */
    static const XMLAttribute CLASS_LVALUE;

    /**
     * \brief Class & motiontask.
     */
    static const XMLAttribute CLASS_MOTIONTASK;

    /**
     * \brief Class & name.
     */
    static const XMLAttribute CLASS_NAME;

    /**
     * \brief Class & operation mode.
     */
    static const XMLAttribute CLASS_OPMODE;

    /**
     * \brief Class & rap-module-info-li.
     */
    static const XMLAttribute CLASS_RAP_MODULE_INFO_LI;

    /**
     * \brief Class & rap-task-li.
     */
    static const XMLAttribute CLASS_RAP_TASK_LI;

    /**
     * \brief Class & RobotWare version name.
     */
    static const XMLAttribute CLASS_RW_VERSION_NAME;

    /**
     * \brief Class & state.
     */
    static const XMLAttribute CLASS_STATE;

    /**
     * \brief Class & sys-option-li.
     */
    static const XMLAttribute CLASS_SYS_OPTION_LI;

    /**
     * \brief Class & sys-system-li.
     */
    static const XMLAttribute CLASS_SYS_SYSTEM_LI;

    /**
     * \brief Class & type.
     */
    static const XMLAttribute CLASS_TYPE;

    /**
     * \brief Class & value.
     */
    static const XMLAttribute CLASS_VALUE;

    /**
     * \brief Class & option.
     */
    static const XMLAttribute CLASS_OPTION;
  };

  /**
   * \brief Identifiers in different RWS messages. E.g. XML attribute names/values.
   */
  struct ABB_LIBRWS_EXPORT Identifiers
  {
    /**
     * \brief Active type.
     */
    static const std::string ACTIVE;

    /**
     * \brief Configuration motion domain type: arm.
     */
    static const std::string ARM;

    /**
     * \brief XML attribute name: class.
     */
    static const std::string CLASS;

    /**
     * \brief Configuration instance list item.
     */
    static const std::string CFG_DT_INSTANCE_LI;

    /**
     * \brief Configuration list item.
     */
    static const std::string CFG_IA_T_LI;

    /**
     * \brief Controller type.
     */
    static const std::string CTRL_TYPE;

    /**
     * \brief Controller execution state.
     */
    static const std::string CTRLEXECSTATE;

    /**
     * \brief Controller state.
     */
    static const std::string CTRLSTATE;

    /**
     * \brief Data type.
     */
    static const std::string DATTYP;

    /**
     * \brief Execution state type.
     */
    static const std::string EXCSTATE;

    /**
     * \brief Home directory.
     */
    static const std::string HOME_DIRECTORY;

    /**
     * \brief IO signal.
     */
    static const std::string IOS_SIGNAL;

    /**
     * \brief Mechanical unit.
     */
    static const std::string MECHANICAL_UNIT;

    /**
     * \brief Mechanical unit group.
     */
    static const std::string MECHANICAL_UNIT_GROUP;

    /**
     * \brief Motion topic in the system configurations (abbreviated as moc).
     */
    static const std::string MOC;

    /**
     * \brief Module name.
     */
    static const std::string MODULE;

    /**
     * \brief Module path.
     */
    static const std::string MODULEPATH;

    /**
     * \brief Motion task.
     */
    static const std::string MOTIONTASK;

    /**
     * \brief Name.
     */
    static const std::string NAME;

    /**
     * \brief Lvalue.
     */
    static const std::string LVALUE;

    /**
     * \brief Opmode.
     */
    static const std::string OPMODE;

    /**
     * \brief Options present on the controller.
     */
    static const std::string PRESENT_OPTIONS;

    /**
     * \brief RAPID module info list item.
     */
    static const std::string RAP_MODULE_INFO_LI;

    /**
     * \brief RAPID task list item.
     */
    static const std::string RAP_TASK_LI;

    /**
     * \brief Restart mode.
     */
    static const std::string RESTART_MODE;

    /**
     * \brief Robot.
     */
    static const std::string ROBOT;

    /**
     * \brief RobotWare version name.
     */
    static const std::string RW_VERSION_NAME;

    /**
     * \brief Single.
     */
    static const std::string SINGLE;

    /**
     * \brief State.
     */
    static const std::string STATE;

    /**
     * \brief Controller topic in the system configurations (abbreviated as sys).
     */
    static const std::string SYS;

    /**
     * \brief Sys option list item.
     */
    static const std::string SYS_OPTION_LI;

    /**
     * \brief Sys system list item.
     */
    static const std::string SYS_SYSTEM_LI;

    /**
     * \brief Title.
     */
    static const std::string TITLE;

    /**
     * \brief Type.
     */
    static const std::string TYPE;

    /**
     * \brief Value.
     */
    static const std::string VALUE;

    /**
     * \brief Option.
     */
    static const std::string OPTION;
  };

  /**
   * \brief RWS queries.
   */
  struct ABB_LIBRWS_EXPORT Queries
  {
    /**
     * \brief Load module action query.
     */
    static const std::string ACTION_LOAD_MODULE;

    /**
     * \brief Release action query.
     */
    static const std::string ACTION_RELEASE;

    /**
     * \brief Request action query.
     */
    static const std::string ACTION_REQUEST;

    /**
     * \brief Reset program pointer action query.
     */
    static const std::string ACTION_RESETPP;

    /**
     * \brief Restart controller.
     */
    static const std::string ACTION_RESTART ;

    /**
     * \brief Set action query.
     */
    static const std::string ACTION_SET;

    /**
     * \brief Set controller state action query.
     */
    static const std::string ACTION_SETCTRLSTATE;

    /**
     * \brief Set locale.
     */
    static const std::string ACTION_SET_LOCALE;

    /**
     * \brief Start action query.
     */
    static const std::string ACTION_START;

    /**
     * \brief Stop action query.
     */
    static const std::string ACTION_STOP;

    /**
     * \brief Unload module action query.
     */
    static const std::string ACTION_UNLOAD_MODULE;

    /**
     * \brief Task query.
     */
    static const std::string TASK;
  };

  /**
   * \brief RWS resources and queries.
   */
  struct ABB_LIBRWS_EXPORT Resources
  {
    /**
     * \brief Instances.
     */
    static const std::string INSTANCES;

    /**
     * \brief Jointtarget.
     */
    static const std::string JOINTTARGET;

    /**
     * \brief Logout.
     */
    static const std::string LOGOUT;

    /**
     * \brief Robtarget.
     */
    static const std::string ROBTARGET;

    /**
     * \brief Configurations.
     */
    static const std::string RW_CFG;

    /**
     * \brief Signals.
     */
    static const std::string RW_IOSYSTEM_SIGNALS;

    /**
     * \brief Mastership.
     */
    static const std::string RW_MASTERSHIP;

    /**
     * \brief Mechanical units.
     */
    static const std::string RW_MOTIONSYSTEM_MECHUNITS;

    /**
     * \brief Panel controller state.
     */
    static const std::string RW_PANEL_CTRLSTATE;

    /**
     * \brief Panel operation mode.
     */
    static const std::string RW_PANEL_OPMODE;

    /**
     * \brief RAPID execution.
     */
    static const std::string RW_RAPID_EXECUTION;

    /**
     * \brief RAPID modules.
     */
    static const std::string RW_RAPID_MODULES;

    /**
     * \brief RAPID symbol data.
     */
    static const std::string RW_RAPID_SYMBOL_DATA_RAPID;

    /**
     * \brief RAPID symbol properties.
     */
    static const std::string RW_RAPID_SYMBOL_PROPERTIES_RAPID;

    /**
     * \brief RAPID tasks.
     */
    static const std::string RW_RAPID_TASKS;

    /**
     * \brief RobotWare system.
     */
    static const std::string RW_SYSTEM;
  };

  /**
   * \brief RWS services.
   */
  struct ABB_LIBRWS_EXPORT Services
  {
    /**
     * \brief Controller service.
     */
    static const std::string CTRL;

    /**
     * \brief Fileservice.
     */
    static const std::string FILESERVICE;

    /**
     * \brief RobotWare service.
     */
    static const std::string RW;

    /**
     * \brief Subscription service.
     */
    static const std::string SUBSCRIPTION;

    /**
     * \brief User service.
     */
    static const std::string USERS;
  };


  /// @brief RWS 2.0 mastership domains.
  ///
  enum class MastershipDomain
  {
    edit,
    motion
  };


  std::ostream& operator<<(std::ostream& os, MastershipDomain domain);


  /// @brief Defines whether the mastership should be implicitly acquired when performing a request.
  ///
  enum class Mastership
  {
    Implicit,
    Explicit
  };


  std::ostream& operator<<(std::ostream& os, Mastership mastership);


  /**
   * \brief Default port number for RWS communication.
   */
  extern const unsigned short DEFAULT_PORT_NUMBER;

  /**
   * \brief Default password (for unconfigured robot controller systems).
   */
  extern const std::string DEFAULT_PASSWORD;

  /**
   * \brief Default username (for unconfigured robot controller systems).
   */
  extern const std::string DEFAULT_USERNAME;
}