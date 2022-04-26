#include <abb_librws/v1_0/rw/cfg.h>

namespace abb :: rws :: v1_0 :: rw :: cfg
{
    void loadCFGFile(RWSClient& client, const FileResource& resource)
    {
        std::stringstream uri;
        uri << Resources::RW_CFG << "/action=load";

        // Path to file should be a direct path, i.e. without "/fileservice/"
        std::string content =
            "filepath=" + resource.directory + "/" + resource.filename + "&action-type=replace";

        client.httpPost(uri.str(), content);
    }
}