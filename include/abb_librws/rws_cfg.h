/***********************************************************************************************************************
 *
 * Copyright (c) 2020, ABB Schweiz AG
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

#ifndef RWS_CFG_H
#define RWS_CFG_H

#include <string>

namespace abb
{
namespace rws
{
/**
 * \brief Namespace for configuration domains.
 */
namespace cfg
{

/**
 * \brief Namespace for types belonging to the motion (MOC) domain.
 */
namespace moc
{
/**
 * \brief Representation of a 'Arm' instance.
 */
struct Arm
{
  /**
   * \brief The instance's name.
   */
  std::string name;

  /**
   * \brief The lower joint bound.
   *
   * The unit depends on the type of joint, i.e. [rad] for rotational and [m] for linear.
   *
   */
  double lower_joint_bound;

  /**
   * \brief The upper joint bound.
   *
   * The unit depends on the type of joint, i.e. [rad] for rotational and [m] for linear.
   */
  double upper_joint_bound;
};
} // end namespace moc

/**
 * \brief Namespace for types belonging to the controller (SYS) domain.
 */
namespace sys
{
/**
 * \brief Representation of a 'Present Option' instance.
 */
struct PresentOption
{
  /**
   * \brief The instance's name.
   */
  std::string name;

  /**
   * \brief The option's description.
   */
  std::string description;
};
} // end namespace sys

} // end namespace cfg
} // end namespace rws
} // end namespace abb

#endif
