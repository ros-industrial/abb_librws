#include <abb_librws/common/rw/rapid.h>

#include <boost/throw_exception.hpp>

#include <stdexcept>
#include <iostream>


namespace abb :: rws :: rw
{
    RAPIDRunMode makeRAPIDRunMode(std::string const& str)
    {
        if (str == "forever")
            return RAPIDRunMode::forever;
        else if (str == "asis")
            return RAPIDRunMode::asis;
        else if (str == "once")
            return RAPIDRunMode::once;
        else if (str == "oncedone")
            return RAPIDRunMode::oncedone;
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Unexpected string representation of RAPID run mode: \"" + str + "\""});
    }


    RAPIDExecutionState makeRAPIDExecutionState(std::string const& str)
    {
        if (str == "running")
            return RAPIDExecutionState::running;
        else if (str == "stopped")
            return RAPIDExecutionState::stopped;
        else
            BOOST_THROW_EXCEPTION(std::invalid_argument {"Unexpected string representation of RAPID execution state: \"" + str + "\""});
    }


    std::ostream& operator<<(std::ostream& os, RAPIDExecutionState const& state)
    {
        switch (state)
        {
        case RAPIDExecutionState::stopped:
            os << "stopped";
            break;

        case RAPIDExecutionState::running:
            os << "running";
            break;
        }

        return os;
    }

}