#include <abb_librws/mastership.h>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>


namespace abb :: rws
{
    Mastership::Mastership(MastershipManager& mastership_manager, std::string const& type)
    :   mastership_manager_ {mastership_manager}
    ,   type_ {type}
    {
        try
        {
            mastership_manager_.requestMastership(type_);
        }
        catch (const boost::exception& e)
        {
            std::cout << boost::diagnostic_information(e) << std::endl;
            throw;
        }
    }

    Mastership::~Mastership()
    {
        try
        {
            mastership_manager_.releaseMastership(type_);
        }
        catch (...)
        {
            std::cerr << boost::current_exception_diagnostic_information() << std::endl;
        }
    }
}
