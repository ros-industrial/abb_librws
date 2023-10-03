#include <abb_librws/v2_0/rw/elog.h>
#include <abb_librws/parsing.h>

#include <boost/throw_exception.hpp>
#include <boost/algorithm/string.hpp>

#include <Poco/DOM/Node.h>
#include <Poco/DOM/DOMWriter.h>

#include <iostream>
#include <stdexcept>

namespace abb ::rws ::v2_0 ::rw ::elog
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

    std::string concatTextFromNodes(std::vector<Poco::XML::Node*> nodes, std::string separator = " ")
    {
        std::stringstream ss;
        bool separator_flag = false;
        for (auto node : nodes)
        {
            if (separator_flag)
                ss << separator;
            separator_flag = true;
            ss << node->innerText();
        }
        return ss.str();
    }

    ElogMessage parseElogNode(Poco::XML::Node* node)
    {
        ElogMessage message;

        std::string title = xmlNodeGetAttributeValue(node, "title");
        int sep_pos = title.find_last_of("/");

        message.domain = 0;
        message.sequenceNumber = 0;        
        if (sep_pos != std::string::npos)
        {
            try {message.domain = std::stoi(title.substr(9, sep_pos - 9));}
            catch (std::out_of_range const& e) {}
            catch (std::bad_alloc const& e) {}
            catch (std::invalid_argument const& e) {}

            try {message.sequenceNumber = std::stoi(title.substr(sep_pos + 1));}
            catch (std::out_of_range const& e) {}
            catch (std::bad_alloc const& e) {}
            catch (std::invalid_argument const& e) {}
        }
        
        message.messageType = makeElogMessageType(std::stoi(xmlFindTextContent(node, XMLAttribute("class", "msgtype"))));
        message.code = std::stoi(xmlFindTextContent(node, XMLAttribute("class", "code")));
        message.sourceName = xmlFindTextContent(node, XMLAttribute("class", "src-name"));
        std::istringstream ss(xmlFindTextContent(node, XMLAttribute("class", "tstamp")));
        ss >> std::get_time(&message.timestamp, "%Y-%m-%d T %H:%M:%S");
        message.title = concatTextFromNodes(xmlFindNodes(node, XMLAttribute("class", "title")));
        message.desc = concatTextFromNodes(xmlFindNodes(node, XMLAttribute("class", "desc")));
        message.conseqs = concatTextFromNodes(xmlFindNodes(node, XMLAttribute("class", "conseqs")));
        message.causes = concatTextFromNodes(xmlFindNodes(node, XMLAttribute("class", "causes")));
        message.actions = concatTextFromNodes(xmlFindNodes(node, XMLAttribute("class", "actions")));

        int argc = std::stoi(xmlFindTextContent(node, XMLAttribute("class", "argc")));

        for (int i = 1; i <= argc; i++)
        {
            std::stringstream arg_name;
            arg_name << "arg" << i;
            auto arg_nodes = xmlFindNodes(node, XMLAttribute("class", arg_name.str()));
            std::string value = "";
            std::string valueType = "STRING";
            if (!arg_nodes.empty())
            {
                value = arg_nodes[0]->innerText();
                valueType = xmlNodeGetAttributeValue(arg_nodes[0], "type");
            }
            message.argv[i] = makeElogMessageArg(valueType, value);
        }
        return message;
    }

    std::vector<ElogMessage> parseElogMessagesXml(std::string const xml_string)
    {
        Poco::AutoPtr<Poco::XML::Document> rws_result = parseXml(xml_string);
        std::vector<ElogMessage> messages;
        auto nodes = xmlFindNodes(rws_result, XMLAttribute("class", "elog-message-li"));
        for (auto node : nodes)
            messages.push_back(parseElogNode(node));
        return messages;
    }

    std::vector<ElogMessage> getElogMessages(RWSClient& client, int const domain, int const seqnum, std::string const& lang, int const limit)
    {
        std::stringstream uri;
        uri << "/rw/elog/" << domain << "?lang=" << lang << "&order=lifo";
        if (seqnum >= 0) uri << "&elogseqnum=" << seqnum;
        if (limit > 0) uri << "&limit=" << limit;
        POCOResult poco_result = client.httpGet(uri.str());
        std::vector<ElogMessage> messages = parseElogMessagesXml(poco_result.content());
        return messages;
    }
}
