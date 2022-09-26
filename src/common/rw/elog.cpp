#include <abb_librws/rws.h>
#include <abb_librws/common/rw/elog.h>
#include <abb_librws/parsing.h>

#include <boost/throw_exception.hpp>
#include <boost/algorithm/string.hpp>

#include <iomanip>
#include <sstream>
#include <variant>

namespace abb ::rws ::rw ::elog
{
    ElogMessageType makeElogMessageType(int val)
    {
        switch (val)
        {
        case 1:
            return ElogMessageType::INFORMATION;
        case 2:
            return ElogMessageType::WARNING;
        case 3:
            return ElogMessageType::ERROR;
        default:
            std::stringstream msg;
            msg << "Unable to make ElogMessageType, invalid value: " << val;
            BOOST_THROW_EXCEPTION(std::invalid_argument(msg.str()));
        }
    }

    std::string to_string(ElogMessageType const messageType)
    {
        switch (messageType)
        {
        case ElogMessageType::INFORMATION:
            return "INFORMATION";
            break;
        case ElogMessageType::WARNING:
            return "WARNING";
            break;
        case ElogMessageType::ERROR:
            return "ERROR";
            break;
        default:
            BOOST_THROW_EXCEPTION(std::logic_error{"Invalid ElogMessageType value"});
        }
    }

    ElogMessageArg makeElogMessageArg(std::string const &type, std::string const& value)
    {
        std::string name = boost::algorithm::to_upper_copy(type);
        if (name == "STRING")
            return ElogMessageArg(value);
        else if (name == "LONG")
            return ElogMessageArg(std::stol(value));
        else if (name == "FLOAT")
            return ElogMessageArg(std::stof(value));
        BOOST_THROW_EXCEPTION(std::invalid_argument("Unexpected string representation of elog message argument type: \"" + type + "\" value: \"" + value + "\""));
    }

    std::string to_string(ElogMessageArg const &arg)
    {
        std::stringstream ss;
        switch (arg.index())
        {
        case 0:
            ss << "STRING";
            break;
        case 1:
            ss << "LONG";
            break;
        case 2:
            ss << "FLOAT";
            break;
        default:
            BOOST_THROW_EXCEPTION(std::logic_error{"Invalid ArgType value"});
        }
        ss << "(";
        std::visit([&ss](auto&& arg){ss << arg;}, arg);
        ss << ")";
        return ss.str();
    }


    const std::string Language::CZECH = "cs";
    const std::string Language::DANISH = "da";
    const std::string Language::GERMAN = "de";
    const std::string Language::ENGLISH = "en";
    const std::string Language::SPANISH = "es";
    const std::string Language::FINNISH = "fi";
    const std::string Language::FRENCH = "fr";
    const std::string Language::HUNGARIAN = "hu";
    const std::string Language::ITALIAN = "it";
    const std::string Language::JAPANESE = "ja";
    const std::string Language::KOREAN = "ko";
    const std::string Language::DUTCH = "nl";
    const std::string Language::POLISH = "pl";
    const std::string Language::PORTUGUESE = "pt";
    const std::string Language::ROMANIAN = "ro";
    const std::string Language::RUSSIAN = "ru";
    const std::string Language::SLOVENIAN = "sl";
    const std::string Language::SWEDISH = "sv";
    const std::string Language::TURKISH = "tr";
    const std::string Language::CHINESE = "zh";

}
