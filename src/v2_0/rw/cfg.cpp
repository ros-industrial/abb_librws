#include <abb_librws/v2_0/rw/cfg.h>

namespace abb :: rws :: v2_0 :: rw :: cfg
{
    std::string loadCFGFile(RWSClient& client, const FileResource& resource, Mastership const& mastership)
    {
        std::stringstream uri;
        uri << Resources::RW_CFG << "/load?mastership=" << mastership;

        // Path to file should be a direct path, i.e. without "/fileservice/"
        std::string content =
            "filepath=" + resource.directory + "/" + resource.filename + "&action-type=replace";
        std::string content_type = "application/x-www-form-urlencoded;v=2.0";


        POCOResult result = client.httpPost(uri.str(), content, content_type);

        std::string progress_id;

        auto const loc = std::find_if(
            result.headerInfo().begin(), result.headerInfo().end(),
            [] (auto it) { return it.first == "Location"; });

        if(loc != result.headerInfo().end())
        {
            std::string const progress = "/progress/";
            auto const start_postion = loc->second.find(progress);

            if (start_postion != std::string::npos)
                progress_id = loc->second.substr(start_postion + progress.size());
        }
        else
            BOOST_THROW_EXCEPTION(ProtocolError {"CFGFile progress id not found."});
        return progress_id;
    }
}