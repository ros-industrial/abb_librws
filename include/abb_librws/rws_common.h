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

#ifndef RWS_COMMON_H
#define RWS_COMMON_H

#include <string>
#include <vector>

#include "Poco/DOM/AutoPtr.h"
#include "Poco/DOM/Document.h"

namespace abb
{
namespace rws
{
/**
 * \brief A struct for representing XML attributes.
 */
struct XMLAttribute
{
  /**
   * \brief A default constructor.
   */
  XMLAttribute() {}
  
  /**
   * \brief A constructor.
   * 
   * \param name for the attribute's name.
   * \param value for the attribute's value.
   */
  XMLAttribute(std::string name, std::string value) : name(name), value(value) {}
  
  /**
   * \brief The name of the attribute.
   */
  std::string name;
  
  /**
   * \brief The value of the attribute.
   */
  std::string value;
};

/**
 * \brief A method for finding all nodes in a XML document that has the specifed attribute (name and value).
 * 
 * \param p_xml_document for the XML document to search.
 * \param attribute specifying the XML attribute (name and value) that the XML node should have.
 *
 * \return std::vector<Poco::XML::Node*> containing the found nodes.
 */
std::vector<Poco::XML::Node*> xmlFindNodes(Poco::AutoPtr<Poco::XML::Document> p_xml_document,
                                           const XMLAttribute attribute);

/**
 * \brief A method for finding the text content of a XML node in a XML document. It stops after the first hit.
 * 
 * \param p_xml_document for the XML document to search.
 * \param attribute specifying the XML attribute (name and value) that the XML text node should have.
 *
 * \return std::string containing the text content. Empty if none were found.
 */
std::string xmlFindTextContent(Poco::AutoPtr<Poco::XML::Document> p_xml_document, const XMLAttribute attribute);

/**
 * \brief A method for finding the text content of a XML node. If not found, then it checks the node's children as well.
 * 
 * \param p_node for the XML node to search.
 * \param attribute specifying the XML attribute (name and value) that the XML text node (child) should have.
 *
 * \return std::string containing the text content. Empty if none were found.
 */
std::string xmlFindTextContent(const Poco::XML::Node* p_node, const XMLAttribute attribute);

/**
 * \brief A method for checking if a XML node has the specified attribute.
 * 
 * \param p_node for the XML node to check.
 * \param attribute specifying the XML attribute (name and value) to check for.
 *
 * \return bool indicating if the attribute was found or not.
 */
bool xmlNodeHasAttribute(const Poco::XML::Node* p_node, const XMLAttribute attribute);

/**
 * \brief Struct containing various constant values defined by default robot controller systems.
 */
struct SystemConstants
{ 
  /**
   * \brief Controller states related constants.
   */
  struct ContollerStates
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
  struct General
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
  };

  /**
   * \brief IO signal related constants.
   */
  struct IOSignals
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
  struct RAPID
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

  /**
   * \brief Robot Web Services (RWS) related constants.
   */
  struct RWS
  {
    /**
     * \brief XML attributes specifying names with corresponding values.
     */
    struct XMLAttributes
    {
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
    };
    
    /**
     * \brief Identifiers in different RWS messages. E.g. XML attribute names/values.
     */
    struct Identifiers
    {
      /**
       * \brief XML attribute name: class.
       */
      static const std::string CLASS;

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
       * \brief Home directory.
       */
      static const std::string HOME_DIRECTORY;
      
      /**
       * \brief IO signal.
       */
      static const std::string IOS_SIGNAL;

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
       * \brief RAPID task list item.
       */
      static const std::string RAP_TASK_LI;
      
      /**
       * \brief RobotWare version name.
       */
      static const std::string RW_VERSION_NAME;
      
      /**
       * \brief State.
       */
      static const std::string STATE;
      
      /**
       * \brief Sys system list item.
       */
      static const std::string SYS_SYSTEM_LI;

      /**
       * \brief Type.
       */
      static const std::string TYPE;

      /**
       * \brief Value.
       */
      static const std::string VALUE;
    };

    /**
     * \brief RWS queries.
     */
    struct Queries
    {
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
    };

    /**
     * \brief RWS resources and queries.
     */
    struct Resources
    {
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
    struct Services
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
  };
};

/**
 * \brief A class for a three-valued boolean.
 */
class TriBool
{
public:
  /**
   * \brief An enum for the different accepted values.
   */
  enum Values
  {
    UNKNOWN_VALUE, ///< Unknown value. E.g. in case of communication failure.
    TRUE_VALUE,    ///< True value.
    FALSE_VALUE    ///< False value.
  };

  /**
   * \brief A default constructor.
   */
  TriBool() : value(UNKNOWN_VALUE) {}

  /**
   * \brief A constructor.
   */
  TriBool(const Values initial_value) : value(initial_value) {}

  /**
   * \brief A constructor.
   *
   * \param value for the initial true or false value.
   */
  TriBool(const bool initial_value) : value(initial_value ? TRUE_VALUE : FALSE_VALUE) {}

  /**
   * \brief Operator for copy assignment.
   *
   * \param rhs for right hand side value.
   *
   * \return TriBool& self.
   */
  TriBool& operator=(const TriBool& other)
  {
    if (&other == this)
    {
      return *this;
    }

    value = other.value;

    return *this;
  }

  /**
   * \brief Operator for assignment.
   *
   * \param rhs for right hand side value.
   *
   * \return TriBool& self.
   */
  TriBool& operator=(const Values& rhs)
  {
    value = rhs;

    return *this;
  }

  /**
   * \brief Operator for assignment.
   *
   * \param rhs for right hand side value.
   *
   * \return TriBool& self.
   */
  TriBool& operator=(const bool& rhs)
  {
    value = (rhs ? TRUE_VALUE : FALSE_VALUE);

    return *this;
  }

  /**
   * \brief Operator for equal to comparison.
   *
   * \param rhs for right hand side value.
   *
   * \return bool indicating if the comparision was equal or not.
   */
  bool operator==(const TriBool& rhs) const
  {
    return value == rhs.value;
  }

  /**
   * \brief Operator for equal to comparison.
   *
   * \param rhs for right hand side value.
   *
   * \return bool indicating if the comparision was equal or not.
   */
  bool operator==(const Values& rhs) const
  {
    return value == rhs;
  }

  /**
   * \brief Operator for not equal to comparison.
   *
   * \param rhs for right hand side value.
   *
   * \return bool indicating if the comparision was not equal or not.
   */
  bool operator!=(const TriBool& rhs) const
  {
    return value != rhs.value;
  }

  /**
   * \brief Operator for not equal to comparison.
   *
   * \param rhs for right hand side value.
   *
   * \return bool indicating if the comparision was not equal or not.
   */
  bool operator!=(const Values& rhs) const
  {
    return value != rhs;
  }

  /**
   * \brief Operator for stream insertion.
   *
   * \param stream for the output stream.
   * \param rhs for the right hand side value, to insert into the output stream.
   *
   * \return std::ostream& for the output stream.
   */
  friend std::ostream& operator<<(std::ostream& stream, const TriBool& rhs)
  {
    return stream << rhs.toString();
  }

  /**
   * \brief A method for checking if the tri bool is unknown.
   *
   * \return bool indicating if the tri bool is unknown or not.
   */
  bool isUnknown() const
  {
    return value == UNKNOWN_VALUE;
  }

  /**
   * \brief A method for checking if the tri bool is true.
   *
   * \return bool indicating if the tri bool is true or not.
   */
  bool isTrue() const
  {
    return value == TRUE_VALUE;
  }

  /**
   * \brief A method for checking if the tri bool is false.
   *
   * \return bool indicating if the tri bool is false or not.
   */
  bool isFalse() const
  {
    return value == FALSE_VALUE;
  }

  /**
   * \brief A method for converting the tri bool to a text string.
   *
   * \return std::string containing the string representation of the value.
   */
  std::string toString() const
  {
    return (isUnknown() ? "unknown" : (isTrue() ? "true" : "false"));
  }

private:
  /**
   * \brief The tri bool value.
   */
  Values value;
};

} // end namespace rws
} // end namespace abb

#endif
