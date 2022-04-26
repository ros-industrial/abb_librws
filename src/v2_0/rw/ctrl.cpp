#include <abb_librws/v2_0/rw/ctrl.h>

namespace abb :: rws :: v2_0 :: rw :: ctrl
{
    void restartController(RWSClient& client, Mastership const& mastership)
    {
        std::stringstream uri;
        uri << Services::CTRL << "/restart?mastership=" << mastership;

        std::string content = "restart-mode=restart";

        std::string content_type = "application/x-www-form-urlencoded;v=2.0";

        client.httpPost(uri.str(), content, content_type);
    }
}