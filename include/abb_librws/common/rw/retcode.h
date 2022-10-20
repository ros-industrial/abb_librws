#pragma once

#include <string>

namespace abb::rws::rw::retcode
{
    /**
     * \brief A structure for containing response of recode resource.
     */
    struct RetcodeInfo
    {
        int code;                /** \brief The error code or success code as number */
        std::string name;        /** \brief The name of error code or success code */
        std::string severity;    /** \brief Success, Warning or Error */
        std::string description; /** \brief Short description of the error code */
    };

    std::string to_string(RetcodeInfo const& retcode);
}

std::ostream& operator<<(std::ostream& os, abb::rws::rw::retcode::RetcodeInfo const& info);
