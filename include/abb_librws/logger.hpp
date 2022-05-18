#pragma once

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>

#include <string>


namespace abb :: rws :: logging
{
    namespace logging = boost::log;
    namespace src = boost::log::sources;
    namespace expr = boost::log::expressions;
    namespace keywords = boost::log::keywords;

    void setupLogger(std::string const& progName);
}