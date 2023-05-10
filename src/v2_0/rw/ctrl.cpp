#include <abb_librws/v2_0/rw/ctrl.h>

namespace abb :: rws :: v2_0 :: rw :: ctrl
{
    void restartController(RWSClient& client, Mastership const& mastership, RestartMode const& restartMode)
    {
        std::stringstream uri;
        uri << Services::CTRL << "/restart?mastership=" << mastership;

        std::stringstream content;
        content << "restart-mode=" << restartMode;

        std::string content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPost(uri.str(), content.str(), content_type);
    }


    SafetyViolationInfo getSafetyViolationInfo(RWSClient& client)
    {
        std::stringstream uri;
        uri << Services::CTRL << "/safety/violation";

        RWSResult rws_result = parseXml(client.httpGet(uri.str()).content());
        
        SafetyViolationInfo result;
        result.unsynchronized = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "unsynchronized"))) != 0;
        result.toolPosViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "tool-pos-violation-status"))) != 0;
        result.armViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "upper-arm-violation-status"))) != 0;
        result.axisRangeViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "axis-range-violation-status"))) != 0;

        return result;
    }

}
