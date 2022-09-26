#include <abb_librws/v1_0/rw/elog.h>
#include <abb_librws/parsing.h>

#include <boost/throw_exception.hpp>
#include <boost/algorithm/string.hpp>

#include <Poco/DOM/Node.h>
#include <Poco/DOM/DOMWriter.h>

#include <iostream>
#include <stdexcept>

namespace abb ::rws ::v1_0 ::rw ::elog
{
    using namespace rws::rw::elog;

    ElogSubscribableResource::ElogSubscribableResource(int domain)
        : domain_(domain)
    {
    }

    std::string ElogSubscribableResource::getURI() const
    {
        std::stringstream ss;
        ss << "/rw/elog/" << domain_;
        return ss.str();
    }

    void ElogSubscribableResource::processEvent(Poco::XML::Element const &li_element, SubscriptionCallback &callback) const
    {
        if (li_element.getAttribute("class") != "elog-message-ev")
            return;

        std::string uri;

        auto nodes = li_element.childNodes();
        for (int i = 0; i < nodes->length(); i++)
        {
            auto node = nodes->item(i);
            if (xmlNodeHasAttribute(node, "rel", "self"))
            {
                uri = xmlNodeGetAttributeValue(node, "href");
                break;
            }
        }
        nodes->release();

        if (uri.length() == 0)
        {
            Poco::XML::DOMWriter writer;
            std::stringstream msg("Unable to process Elog subscription event from XML Element: ");
            writer.writeNode(msg, &li_element);
            BOOST_THROW_EXCEPTION(std::invalid_argument(msg.str()));
        }

        std::string const prefix = "/rw/elog/";
        if (uri.find(prefix) != 0)
            BOOST_THROW_EXCEPTION(std::invalid_argument{"Unable to process Elog subscription event: invalid resource URI \"" + uri + "\""});

        std::vector<std::string> parts;
        boost::split(parts, uri.substr(prefix.size()), boost::is_any_of("/"));

        if (parts.size() < 2)
            BOOST_THROW_EXCEPTION(std::invalid_argument{"Unable to process Elog subscription event: unable to find domain and sequence number in uri: \"" + uri + "\""});

        ElogEvent event;
        event.domain = std::stoi(parts[0]);
        event.seqnum = std::stoi(parts[1]);
        event.resource = std::make_shared<ElogSubscribableResource>(*this);
        callback.processEvent(event);
    }

    std::vector<std::pair<int, std::string>> parseDomains(RWSResult const &rws_result)
    {
        std::vector<Poco::XML::Node *> domains_xml = xmlFindNodes(rws_result, XMLAttribute("class", "elog-domain-li"));
        std::vector<std::pair<int, std::string>> elogDomains;
        for (auto node : domains_xml)
        {
            std::pair<int, std::string> domain;
            domain.first = std::stoi(xmlNodeGetAttributeValue(node, "title"));
            domain.second = xmlFindTextContent(node, XMLAttribute("class", "domain-name"));
            elogDomains.push_back(domain);
        }
        return elogDomains;
    }

    std::vector<std::pair<int, std::string>> getElogDomains(RWSClient &client, std::string lang)
    {
        std::stringstream uri;
        uri << "/rw/elog?lang=" << lang << "&resource=count";
        POCOResult poco_result = client.httpGet(uri.str());
        RWSResult const &rws_result = parseXml(poco_result.content());
        return parseDomains(rws_result);
    }

    ElogMessage parseElogMessageXml(std::string const xml_string)
    {
        Poco::AutoPtr<Poco::XML::Document> rws_result = parseXml(xml_string);
        ElogMessage message;
        message.messageType = makeElogMessageType(std::stoi(xmlFindTextContent(rws_result, XMLAttribute("class", "msgtype"))));
        message.code = std::stoi(xmlFindTextContent(rws_result, XMLAttribute("class", "code")));
        message.sourceName;
        std::istringstream ss(xmlFindTextContent(rws_result, XMLAttribute("class", "tstamp")));
        ss >> std::get_time(&message.timestamp, "%Y-%m-%d T %H:%M:%S");
        message.title = xmlFindTextContent(rws_result, XMLAttribute("class", "title"));
        message.desc = xmlFindTextContent(rws_result, XMLAttribute("class", "desc"));
        message.conseqs = xmlFindTextContent(rws_result, XMLAttribute("class", "conseqs"));
        message.causes = xmlFindTextContent(rws_result, XMLAttribute("class", "causes"));
        message.actions = xmlFindTextContent(rws_result, XMLAttribute("class", "actions"));

        int argc = std::stoi(xmlFindTextContent(rws_result, XMLAttribute("class", "argc")));

        for (int i = 1; i <= argc; i++)
        {
            std::stringstream arg_name;
            arg_name << "arg" << i;
            auto node = xmlFindNodes(rws_result, XMLAttribute("class", arg_name.str()))[0];

            std::string value = node->innerText();
            std::string valueType = xmlNodeGetAttributeValue(node, "type");

            message.argv[i] = makeElogMessageArg(valueType, value);
        }
        return message;
    }

    ElogMessage getElogMessage(RWSClient &client, int const domain, int const seqnum, std::string const &lang)
    {
        std::stringstream uri;
        uri << "/rw/elog/" << domain << "/" << seqnum << "?lang=" << lang;
        POCOResult poco_result = client.httpGet(uri.str());
        ElogMessage message = parseElogMessageXml(poco_result.content());
        message.domain = domain;
        message.sequenceNumber = seqnum;
        return message;
    }
}
