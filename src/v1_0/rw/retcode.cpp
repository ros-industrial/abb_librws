#include <abb_librws/v1_0/rw/retcode.h>


namespace abb :: rws :: v1_0 :: rw :: retcode
{
    RetcodeInfo getRetcodeInfo(RWSClient& client, int code)
    {
        std::stringstream uri;
        uri << "/rw/retcode?code=" << code;
        RWSResult const& rws_result = parseXml(client.httpGet(uri.str()).content());
        RetcodeInfo retcode_info;
        retcode_info.code = std::stoi(xmlNodeTextByTagAndAttribute(rws_result, "span", abb::rws::XMLAttribute("class","code")).value());
        retcode_info.name = xmlNodeTextByTagAndAttribute(rws_result, "span", abb::rws::XMLAttribute("class","name")).value();
        retcode_info.severity = xmlNodeTextByTagAndAttribute(rws_result, "span", abb::rws::XMLAttribute("class","severity")).value();
        retcode_info.description = xmlNodeTextByTagAndAttribute(rws_result, "span", abb::rws::XMLAttribute("class","description")).value();
        return retcode_info;
    }
}
