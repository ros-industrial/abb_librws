#include <abb_librws/v1_0/rw/cfg.h>

namespace abb :: rws :: v1_0 :: rw :: cfg
{
    void loadCFGfile(RWSClient& client, const FileResource& resource, Mastership const& mastership)
    {
        std::string uri = Resources::RW_CFG + "/" + Queries::ACTION_LOAD;

        // Path to file should be a direct path, i.e. without "/fileservice/"
        std::string content =
            Identifiers::FILEPATH + "=" + resource.directory + "/" + resource.filename +
            "&" + Identifiers::ACTION_TYPE + "=add";

        client.httpPost(uri, content);
    }
}