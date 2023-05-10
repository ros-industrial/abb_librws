#include <abb_librws/v1_0/rw/ctrl.h>

namespace abb :: rws :: v1_0 :: rw :: ctrl
{
    void restartController(RWSClient& client, RestartMode const& restartMode)
    {
        std::stringstream uri;
        uri << Services::CTRL;

        std::stringstream content;
        content << "restart-mode=" << restartMode;

        client.httpPost(uri.str(), content.str(), {Poco::Net::HTTPResponse::HTTP_OK});
    }

    SafetyViolationInfo getSafetyViolationInfo(RWSClient& client)
    {
        std::stringstream uri;
        uri << Services::CTRL << "/safety/violation";

        RWSResult rws_result = parseXml(client.httpGet(uri.str()).content());
        
        SafetyViolationInfo result;
        result.unsynchronized = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "unsynchronized"))) != 0;
        result.toolPosViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "tool-pos-violation-status")))!= 0;
        result.armViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "upper-arm-violation-status"))) != 0;
        result.axisRangeViolation = std::stoi(xmlFindTextContent(rws_result, XMLAttribute(Identifiers::CLASS, "axis-range-violation-status"))) != 0;

        return result;
    }
}
