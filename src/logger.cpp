#include <abb_librws/logger.hpp>

#include <boost/log/utility/setup/console.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


namespace abb :: rws :: logging
{
    void setupLogger(std::string const& progName)
    {
        boost::shared_ptr<logging::core> core = logging::core::get();
        logging::add_console_log(std::cout, keywords::format = (
        expr::stream
                << "[" << progName << "]"
                << "[" << logging::trivial::severity << "]["
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                << "]: " << expr::smessage
        ));
        core->set_filter
        (
            logging::trivial::severity >= logging::trivial::debug
        );
        logging::add_common_attributes();
    }
}