#include <abb_librws/common/rw/panel.h>

#include <boost/throw_exception.hpp>

#include <stdexcept>
#include <iostream>


namespace abb :: rws :: rw
{
    ControllerState makeControllerState(std::string const& str)
    {
        if (str == "init")
            return ControllerState::init;
        else if (str == "motoron")
            return ControllerState::motorOn;
        else if (str == "motoroff")
            return ControllerState::motorOff;
        else if (str == "guardstop")
            return ControllerState::guardStop;
        else if (str == "emergencystop")
            return ControllerState::emergencyStop;
        else if (str == "emergencystopreset")
            return ControllerState::emergencyStopReset;
        else if (str == "sysfail")
            return ControllerState::sysFail;
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Unexpected string representation of controller state: \"" + str + "\""});
    }


    std::ostream& operator<<(std::ostream& os, ControllerState state)
    {
        switch (state)
        {
        case ControllerState::init:
            os << "init";
            break;
        case ControllerState::motorOn:
            os << "motoron";
            break;
        case ControllerState::motorOff:
            os << "motoroff";
            break;
        case ControllerState::guardStop:
            os << "guardstop";
            break;
        case ControllerState::emergencyStop:
            os << "emergencystop";
            break;
        case ControllerState::emergencyStopReset:
            os << "emergencystopreset";
            break;
        case ControllerState::sysFail:
            os << "sysfail";
            break;
        default:
            BOOST_THROW_EXCEPTION(std::logic_error {"Invalid ControllerState value"});
        }

        return os;
    }


    std::ostream& operator<<(std::ostream& os, OperationMode mode)
    {
        switch (mode)
        {
        case OperationMode::init:
            os << "INIT";
            break;
        case OperationMode::autoCh:
            os << "AUTO_CH";
            break;
        case OperationMode::manFCh:
            os << "MANF_CH";
            break;
        case OperationMode::manR:
            os << "MANR";
            break;
        case OperationMode::manF:
            os << "MANF";
            break;
        case OperationMode::automatic:
            os << "AUTO";
            break;
        case OperationMode::undef:
            os << "UNDEF";
            break;
        default:
            BOOST_THROW_EXCEPTION(std::logic_error {"Invalid OperationMode value"});
        }

        return os;
    }


    /**
     * \brief Create \a OperationMode from string.
     *
     * \param str source string
     *
     * \return \a OperationMode matching the value of \a str
     *
     * \throw \a std::invalid_argument if \a str is not from the set of valid strings.
     */
    OperationMode makeOperationMode(std::string const& str)
    {
        if (str == "INIT")
            return OperationMode::init;
        else if (str == "AUTO_CH")
            return OperationMode::autoCh;
        else if (str == "MANF_CH")
            return OperationMode::manFCh;
        else if (str == "MANR")
            return OperationMode::manR;
        else if (str == "MANF")
            return OperationMode::manF;
        else if (str == "AUTO")
            return OperationMode::automatic;
        else if (str == "UNDEF")
            return OperationMode::undef;
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Unexpected string representation of operation mode: \"" + str + "\""});
    }
}