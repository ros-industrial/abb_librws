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

#include <sstream>
#include <iomanip>

#include <abb_librws/system_constants.h>
#include <abb_librws/rws_rapid.h>

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

} // end namespace rws
} // end namespace abb
