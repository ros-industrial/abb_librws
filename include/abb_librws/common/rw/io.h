#pragma once

#include <map>
#include <variant>
#include <string>


namespace abb :: rws :: rw :: io
{
    /**
     * \brief Mapping from IO signal name to a value.
     *
     * The value of a digital signal is a \a bool, the value of an analog signal is a \a float.
     */
    using IOSignalInfo = std::map<std::string, std::variant<bool, float>>;
}