
#include <abb_librws/rws.h>
#include <abb_librws/v2_0/rws_client.h>

#include <string>

namespace abb :: rws :: v2_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v2_0 :: rw :: cfg
{
    /**
     * \brief A function for loading configs to robot controller.
     *
     * https://developercenter.robotstudio.com/api/rwsApi/cfg_system_load_page.html
     *
     * \param client RWS client
     * \param resource specifying the file's directory and name.
     * \param mastership {implicit | explicit} by default mastership is explicit
     *
     * \throw \a RWSError if something goes wrong.
     */
    void loadCFGFile(RWSClient& client, const FileResource& resource,
        Mastership const& mastership = Mastership::Explicit);

}