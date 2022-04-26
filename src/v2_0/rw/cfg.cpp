#include <abb_librws/v2_0/rw/panel.h>
#include <abb_librws/v2_0/rws.h>

namespace abb :: rws :: v2_0 :: rw :: cfg
{
    void loadCFGFile(RWSClient& client, const FileResource& resource, Mastership const& mastership)
    {
        std::stringstream uri;
        uri << Resources::RW_CFG << "/load?mastership=" << mastership;

        // Path to file should be a direct path, i.e. without "/fileservice/"
        std::string content =
            "filepath=" + resource.directory + "/" + resource.filename + "&action-type=replace";
        std::string content_type = "application/x-www-form-urlencoded;v=2.0";


        client.httpPost(uri.str(), content, content_type);
    }
}