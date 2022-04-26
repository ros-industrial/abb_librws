
#include <abb_librws/rws.h>
#include <abb_librws/v1_0/rws_client.h>

#include <string>
#include <iosfwd>

namespace abb :: rws :: v1_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v1_0 :: rw :: cfg
{
    /**
     * \brief A function for loading configs to robot controller.
     *
     * https://developercenter.robotstudio.com/api/rwsApi/cfg_system_load_page.html
     *
     * \param client RWS client
     * \param resource specifying the file's directory and name.
     *
     * \throw \a RWSError if something goes wrong.
     */
    void loadCFGFile(RWSClient& client, const FileResource& resource);

}