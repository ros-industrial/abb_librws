#include <abb_librws/rws.h>

#include <boost/throw_exception.hpp>

#include <stdexcept>
#include <iostream>


namespace abb :: rws
{
  std::ostream& operator<<(std::ostream& os, StopMode stopmode)
  {
    switch (stopmode)
    {
      case StopMode::cycle:
        os << "cycle";
        break;
      case StopMode::instr:
        os << "instr";
        break;
      case StopMode::stop:
        os << "stop";
        break;
      case StopMode::qstop:
        os << "qstop";
        break;
      default:
        BOOST_THROW_EXCEPTION(std::logic_error {"Invalid StopMode value"});
    }

    return os;
  }


  std::ostream& operator<<(std::ostream& os, UseTsp usetsp)
  {
    switch (usetsp)
    {
      case UseTsp::normal:
        os << "normal";
        break;
      case UseTsp::alltsk:
        os << "alltsk";
        break;
      default:
        BOOST_THROW_EXCEPTION(std::logic_error {"Invalid UseTsp value"});
    }

    return os;
  }
}