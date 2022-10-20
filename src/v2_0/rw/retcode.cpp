#include <abb_librws/v2_0/rw/retcode.h>


namespace abb::rws::v2_0::rw::retcode
{
    RetcodeInfo parseErrorDescriptionXml(Poco::XML::Node* node)
    {
        RetcodeInfo retcode_info;
        retcode_info.code = std::stoi(xmlFindTextContent(node, abb::rws::XMLAttribute("class","code")));
        retcode_info.name = xmlFindTextContent(node, abb::rws::XMLAttribute("class","name"));
        retcode_info.severity = xmlFindTextContent(node, abb::rws::XMLAttribute("class","severity"));
        retcode_info.description = xmlFindTextContent(node, abb::rws::XMLAttribute("class","description"));
        return retcode_info;
    }

    RetcodeInfo getRetcodeInfo(RWSClient& client, int code)
    {
        std::stringstream uri;
        uri << "/rw/retcode?code=" << code;
        POCOResult poco_result = client.httpGet(uri.str());
        RWSResult const& rws_result = parseXml(poco_result.content());
        std::vector<Poco::XML::Node*> nodes = xmlFindNodes(rws_result, XMLAttribute("class", "err-desc"));
        if (nodes.empty())
        {
            using content_info = boost::error_info<struct XmlContent, std::string>;
            BOOST_THROW_EXCEPTION(boost::enable_error_info(std::runtime_error("Unable to find error description: "))
                                    << content_info {poco_result.content()});
        }
        return parseErrorDescriptionXml(nodes[0]);
    }

    std::map<int, RetcodeInfo> getRetcodes(RWSClient& client)
    {
        std::string uri = "/rw/retcode";
        POCOResult poco_result = client.httpGet(uri);
        RWSResult const& rws_result = parseXml(poco_result.content());
        std::vector<Poco::XML::Node*> nodes = xmlFindNodes(rws_result, XMLAttribute("class", "err-desc-li"));
        std::map<int,RetcodeInfo> retcodes;
        for (auto node : nodes)
        {
            RetcodeInfo info = parseErrorDescriptionXml(node);
            retcodes[info.code] = info;
        }
        return retcodes;
    }
}
