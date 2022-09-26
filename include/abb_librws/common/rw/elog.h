#pragma once

#include <string>
#include <map>
#include <chrono>
#include <variant>

namespace abb ::rws ::rw ::elog
{

    /**
     * @brief Types of elog messages
     */
    enum class ElogMessageType
    {
        INFORMATION = 1,
        WARNING = 2,
        ERROR = 3
    };

    ElogMessageType makeElogMessageType(int val);

    std::string to_string(ElogMessageType const messagType);


    /**
     * @brief Argument of elog message.
     * Can be of string, long or float type.
     * https://developercenter.robotstudio.com/api/rwsApi/elog_domain_get_page.html
     */
    using ElogMessageArg = std::variant<std::string, long, float>;

    ElogMessageArg makeElogMessageArg(std::string const &type, std::string const& value);

    std::string to_string(ElogMessageArg const& arg);


    /**
     * @brief Elog message structure
     * https://developercenter.robotstudio.com/api/rwsApi/elog_domain_get_page.html
     */
    struct ElogMessage
    {
        int domain;                       // Domain of message.
        int sequenceNumber;               // Sequence number of message in domain.
        ElogMessageType messageType;          // The elog message type.
        int code;                         // The elog message code.
        std::string sourceName;           // The elog message source.
        std::tm timestamp;                // The time stamp when the event log was generated.
        std::string title;                // The elog message title in the specified langauge. Available only when lang parameter is provided.
        std::string desc;                 // The elog message description in the specified langauge. Available only when lang parameter is provided.
        std::string conseqs;              // The elog message consequences in the specified langauge. Available only when lang parameter is provided.
        std::string causes;               // The elog message causes in the specified langauge. Available only when lang parameter is provided.
        std::string actions;              // The elog message actions in the specified langauge. Available only when lang parameter is provided.
        
        /**
         * @brief Message arguments necessary for filling message templates.
         * If valid language is provided when retrieving /rw/elog message, args are filled in appropirate places. 
         * @param first arg name: e.g. arg1, arg2 etc.. 
         * @param second the argument value. The type of argument can be float, string or long.
         */
        std::map<int,ElogMessageArg> argv;
    };

    struct Language final
    {
        static const std::string CZECH;
        static const std::string DANISH;
        static const std::string GERMAN;
        static const std::string ENGLISH;
        static const std::string SPANISH;
        static const std::string FINNISH;
        static const std::string FRENCH;
        static const std::string HUNGARIAN;
        static const std::string ITALIAN;
        static const std::string JAPANESE;
        static const std::string KOREAN;
        static const std::string DUTCH;
        static const std::string POLISH;
        static const std::string PORTUGUESE;
        static const std::string ROMANIAN;
        static const std::string RUSSIAN;
        static const std::string SLOVENIAN;
        static const std::string SWEDISH;
        static const std::string TURKISH;
        static const std::string CHINESE;
    };
}
