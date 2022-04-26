#include <abb_librws/v1_0/rw/ctrl.h>

namespace abb :: rws :: v1_0 :: rw :: ctrl
{
    void restartController(RWSClient& client)
    {
        std::stringstream uri;
        uri << Services::CTRL;

        std::string content = "restart-mode=restart";

        client.httpPost(uri.str(), content);
    }
}