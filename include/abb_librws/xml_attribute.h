#pragma once

#include <string>


namespace abb :: rws
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
  XMLAttribute(const std::string& name, const std::string& value) : name(name), value(value) {}

  /**
   * \brief The name of the attribute.
   */
  std::string name;

  /**
   * \brief The value of the attribute.
   */
  std::string value;
  };
}