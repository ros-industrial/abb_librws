
#include <abb_librws/rws.h>

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
     *
     * \return Current RAPID execution state.
     *
     * \throw \a RWSError if something goes wrong.
     */
    void loadCFGfile(RWSClient& client, const FileResource& resource, Mastership const& mastership)

}