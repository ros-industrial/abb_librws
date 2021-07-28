#pragma once


namespace abb :: rws
{
  /**
   * \brief An enumeration of controller coordinate frames.
   */
  enum class Coordinate
  {
    BASE,  ///< \brief Base frame coordinate.
    WORLD, ///< \brief World frame coordinate.
    TOOL,  ///< \brief Tool frame coordinate.
    WOBJ,  ///< \brief Work object (wobj) frame coordinate.
    ACTIVE ///< \brief Currently active coordinate.
  };
}