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
}