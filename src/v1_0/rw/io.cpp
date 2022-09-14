#include <abb_librws/v1_0/rw/io.h>
#include <abb_librws/parsing.h>

#include <boost/throw_exception.hpp>


namespace abb :: rws :: v1_0 :: rw :: io
{
    static std::string generateIOSignalPath(const std::string& iosignal);
    static bool digitalSignalToBool(std::string const& value);


    IOSignalInfo getIOSignals(RWSClient& client)
    {
        auto const doc = parseXml(client.httpGet(Resources::RW_IOSYSTEM_SIGNALS).content());

        IOSignalInfo signals;

        for (auto&& node : xmlFindNodes(doc, {"class", "ios-signal-li"}))
        {
            std::string const name = xmlFindTextContent(node, {"class", "name"});
            std::string const value = xmlFindTextContent(node, {"class", "lvalue"});
            std::string const type = xmlFindTextContent(node, {"class", "type"});

            if (!name.empty() && !value.empty())
            {
                if (type == "DI" || type == "DO")
                    signals[name] = digitalSignalToBool(value);
                else if (type == "AI" || type == "AO")
                    signals[name] = std::stof(value);
            }
        }

        return signals;
    }


    bool getDigitalSignal(RWSClient& client, std::string const& signal_name)
    {
        return digitalSignalToBool(getIOSignal(client, signal_name));
    }


    float getAnalogSignal(RWSClient& client, std::string const& signal_name)
    {
        return std::stof(getIOSignal(client, signal_name));
    }


    std::uint32_t getGroupSignal(RWSClient& client, std::string const& signal_name)
    {
        return std::stoul(getIOSignal(client, signal_name));
    }


    void setDigitalSignal(RWSClient& client, std::string const& signal_name, bool value)
    {
        setIOSignal(client, signal_name, value ? SystemConstants::IOSignals::HIGH : SystemConstants::IOSignals::LOW);
    }


    void setAnalogSignal(RWSClient& client, std::string const& signal_name, float value)
    {
        std::stringstream str;
        str << std::setprecision(SINGLE_PRECISION_DIGITS) << value;
        setIOSignal(client, signal_name, str.str());
    }


    void setGroupSignal(RWSClient& client, std::string const& signal_name, std::uint32_t value)
    {
        setIOSignal(client, signal_name, std::to_string(value));
    }


    std::string getIOSignal(RWSClient& client, const std::string& iosignal)
    {
        try
        {
            std::string const uri = generateIOSignalPath(iosignal);
            auto const rws_result = parseXml(client.httpGet(uri).content());

            return xmlFindTextContent(rws_result, XMLAttributes::CLASS_LVALUE);
        }
        catch (boost::exception& e)
        {
            e << IoSignalErrorInfo {iosignal};
            throw;
        }
    }


    void setIOSignal(RWSClient& client, const std::string& iosignal, const std::string& value)
    {
        try
        {
            std::string uri = generateIOSignalPath(iosignal) + "?" + Queries::ACTION_SET;
            std::string content = Identifiers::LVALUE + "=" + value;

            client.httpPost(uri, content);
        }
        catch (boost::exception& e)
        {
            e << IoSignalErrorInfo {iosignal};
            throw;
        }
    }


    static std::string generateIOSignalPath(const std::string& iosignal)
    {
        return Resources::RW_IOSYSTEM_SIGNALS + "/" + iosignal;
    }


    static bool digitalSignalToBool(std::string const& value)
    {
        if (value != SystemConstants::IOSignals::HIGH && value != SystemConstants::IOSignals::LOW)
            BOOST_THROW_EXCEPTION(std::logic_error {"Unexpected value \"" + value + "\" of a digital signal"});

        return value == SystemConstants::IOSignals::HIGH;
    }

    std::string IOSignalSubscribableResource::getURI() const
    {
      std::string resource_uri = Resources::RW_IOSYSTEM_SIGNALS;
      resource_uri += "/";
      resource_uri += name;
      resource_uri += ";";
      resource_uri += Identifiers::STATE;
      return resource_uri;
    }


    void IOSignalSubscribableResource::processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const
    {
        if (li_element.getAttribute("class") == "ios-signalstate-ev")
        {
            Poco::XML::Element const * a_element = li_element.getChildElement("a");
            if (!a_element)
                BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: `li` element has no `a` element"});

            if (a_element->getAttribute("href") == getURI())
            {
                IOSignalStateEvent event;
                event.signal = name;
                event.value = xmlFindTextContent(&li_element, XMLAttribute {"class", "lvalue"});
                event.resource = std::make_shared<IOSignalSubscribableResource>(event.signal);

                callback.processEvent(event);
            }
        }
    }
}