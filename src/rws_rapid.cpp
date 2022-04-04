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

#include <abb_librws/rws_rapid.h>
#include <abb_librws/system_constants.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace abb
{
namespace rws
{
typedef SystemConstants::RAPID RAPID;

/***********************************************************************************************************************
 * Struct definitions: RAPIDAtomic<RAPIDAtomicTypes>
 */

/************************************************************
 * Auxiliary methods
 */

std::string RAPIDAtomic<RAPID_BOOL>::getType() const
{
  return RAPID::TYPE_BOOL;
}

std::string RAPIDAtomic<RAPID_NUM>::getType() const
{
  return RAPID::TYPE_NUM;
}

std::string RAPIDAtomic<RAPID_DNUM>::getType() const
{
  return RAPID::TYPE_DNUM;
}

std::string RAPIDAtomic<RAPID_STRING>::getType() const
{
  return RAPID::TYPE_STRING;
}

std::string RAPIDAtomic<RAPID_BOOL>::constructString() const
{
  return (value ? RAPID::RAPID_TRUE : RAPID::RAPID_FALSE);
}

std::string RAPIDAtomic<RAPID_NUM>::constructString() const
{
  std::stringstream ss;

  // Use fixed format for floating point values with absolute value greater than 1,
  // otherwise RWS can complain about exponential notation with a positive exponent.
  if (std::abs(value) > 1.f)
    ss << std::fixed;

  ss << std::setprecision(SINGLE_PRECISION_DIGITS) << value;

  return ss.str();
}

std::string RAPIDAtomic<RAPID_DNUM>::constructString() const
{
  std::stringstream ss;

  // Use fixed format for floating point values with absolute value greater than 1,
  // otherwise RWS can complain about exponential notation with a positive exponent.
  if (std::abs(value) > 1.)
    ss << std::fixed;

  ss << std::setprecision(DOUBLE_PRECISION_DIGITS) << value;

  return ss.str();
}

std::string RAPIDAtomic<RAPID_STRING>::constructString() const
{
  return "\"" + value + "\"";
}

void RAPIDAtomic<RAPID_BOOL>::parseString(const std::string& value_string)
{
  value = value_string.compare(RAPID::RAPID_TRUE) == 0 ? true : false;
}

void RAPIDAtomic<RAPID_STRING>::parseString(const std::string& value_string)
{
  std::string temp = value_string;

  size_t position = temp.find_first_of("\"");
  if (position == 0)
  {
    temp.erase(position, 1);
  }
  position = temp.find_last_of("\"");
  if (position == temp.size() - 1)
  {
    temp.erase(position, 1);
  }

  value = temp;
}




/***********************************************************************************************************************
 * Class definitions: RAPIDRecord
 */

/************************************************************
 * Primary methods
 */

RAPIDRecord::RAPIDRecord(const std::string& record_type_name)
:
record_type_name_(record_type_name)
{}

std::string RAPIDRecord::getType() const
{
  return record_type_name_;
}

void RAPIDRecord::parseString(const std::string& value_string)
{
  std::vector<std::string> substrings = extractDelimitedSubstrings(value_string, '[', ']', ',');

  if (components_.size() == substrings.size())
  {
    for (size_t i = 0; i < components_.size(); ++i)
    {
      components_.at(i)->parseString(substrings.at(i));
    }
  }
}

std::string RAPIDRecord::constructString() const
{
  std::stringstream ss;

  ss << "[";

  for (size_t i = 0; i < components_.size(); ++i)
  {
    ss << components_.at(i)->constructString();

    if (i != components_.size() - 1)
    {
      ss << ",";
    }
  }

  ss << "]";

  return ss.str();
}

RAPIDRecord& RAPIDRecord::operator=(const RAPIDRecord& other)
{
  if (this != &other)
  {
    if (record_type_name_ == other.record_type_name_)
    {
      if (components_.size() == other.components_.size())
      {
        for (size_t i = 0; i < components_.size(); ++i)
        {
          components_.at(i)->parseString(other.components_.at(i)->constructString());
        }
      }
    }
  }

  return *this;
}

std::ostream& operator<<(std::ostream& os, ExtJoint const& extax)
{
    return os
        << "ExtJoint("
        << ".eax_a=" << extax.eax_a.value // external axis a
        << ", .eax_b=" << extax.eax_b.value
        << ", .eax_c=" << extax.eax_c.value
        << ", .eax_d=" << extax.eax_d.value
        << ", .eax_e=" << extax.eax_e.value
        << ", .eax_f=" << extax.eax_f.value
        << ")";
}

std::ostream& operator<<(std::ostream& os, Orient const& orient)
{
    return os
        << "Orient("
        << ".q1=" << orient.q1.value
        << ", .q2=" << orient.q2.value
        << ", .q3=" << orient.q3.value
        << ", .q4=" << orient.q4.value
        << ")";
}

std::ostream& operator<<(std::ostream& os, Pos const& pos)
{
    return os
        << "Pos("
        << ".x=" << pos.x.value
        << ", .y=" << pos.y.value
        << ", .z=" << pos.z.value
        << ")";
}

std::ostream& operator<<(std::ostream& os, ConfData const& robconf)
{
    return os
        << "ConfData("
        << "cf1=" << robconf.cf1.value // quadrent number for axis 1
        << "cf4=" << robconf.cf4.value // quadrent number for axis 4
        << "cf6=" << robconf.cf6.value // quadrent number for axis 6
        << ")";
}

std::ostream& operator<<(std::ostream& os, SpeedData const& speeddata)
{
    return os
        << "SpeedData("
        << ".v_tcp=" << speeddata.v_tcp.value // [mm/s] of TCP
        << ".v_ori=" << speeddata.v_ori.value // [deg/s] of TCP
        << ".v_leax=" << speeddata.v_leax.value // [mm/s] of external axes
        << ".v_reax=" << speeddata.v_reax.value // [deg/s] of external axes
        << ")";
}

std::ostream& operator<<(std::ostream& os, ZoneData const& zonedata)
{
    return os
        << "ZoneData("
        << ".finep=" << zonedata.finep.value // if true, movement terminates at stop point
        << ", .pzone_tcp=" << zonedata.pzone_tcp.value // [mm] radius of the TCP zone
        << ", .pzone_ori=" << zonedata.pzone_ori.value // [mm] radius of the tool reorientation zone
        << ", .pzone_eax=" << zonedata.pzone_eax.value // [mm] radius of the external axes zone
        << ", .zone_ori=" << zonedata.zone_ori.value // [deg] zone for the tool reorientation
        << ", .zone_leax=" << zonedata.zone_leax.value // [mm] zone size for linear external axes
        << ", .zone_reax=" << zonedata.zone_reax.value // [deg] zone size for rotating external axes
        << ")";
}

std::ostream& operator<<(std::ostream& os, RobTarget const& robtarget)
{
    return os
        << "RobTarget("
        << ".pos=" << robtarget.pos
        << ", .orient=" << robtarget.orient
        << ", .robconf=" << robtarget.robconf
        << ", .extax=" << robtarget.extax
        << ")";
}

std::ostream& operator<<(std::ostream& os, LoadData const& loaddata)
{
    return os
        << "LoadData("
        << ".mass=" << loaddata.mass.value // [kg]
        << ", .cog=" << loaddata.cog // center of gravity
        << ", .aom=" << loaddata.aom // axes of moment
        << ", .ix=" << loaddata.ix.value // inertia around x-axis [kgm^2]
        << ", .iy=" << loaddata.iy.value // inertia around y-axis [kgm^2]
        << ", .iz=" << loaddata.iz.value // inertia around z-axis [kgm^2]
        << ")";
}

std::ostream& operator<<(std::ostream& os, Pose const& pose)
{
    return os
        << "Pose("
        << ".pos=" << pose.pos
        << ".rot=" << pose.rot
        << ")";
}


std::ostream& operator<<(std::ostream& os, ToolData const& tooldata)
{
    return os
        << "ToolData("
        << ".robhold=" << tooldata.robhold.value // is robot holding the tool
        << ", .tframe=" << tooldata.tframe // tool's coordinate system
        << ", .tload=" << tooldata.tload // tool's load
        << ")";
}

} // end namespace rws
} // end namespace abb
