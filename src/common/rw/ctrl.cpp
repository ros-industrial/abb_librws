#include <abb_librws/common/rw/ctrl.h>

#include <boost/throw_exception.hpp>

#include <stdexcept>
#include <iostream>


namespace abb :: rws :: rw
{
    std::ostream& operator<<(std::ostream& os, RestartMode mode)
    {
        switch (mode)
        {
        case RestartMode::restart:
            os << "restart";
            break;
        case RestartMode::shutdown:
            os << "shutdown";
            break;
        case RestartMode::xstart:
            os << "xstart";
            break;
        case RestartMode::istart:
            os << "istart";
            break;
        case RestartMode::pstart:
            os << "pstart";
            break;
        case RestartMode::bstart:
            os << "bstart";
            break;
        default:
            BOOST_THROW_EXCEPTION(std::logic_error {"Invalid RestartMode value"});
        }

        return os;
    }
}