
#include <abb_librws/rws.h>
#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/common/rw/ctrl.h>

#include <string>
#include <iosfwd>

namespace abb :: rws :: v2_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v2_0 :: rw :: ctrl
{
    /**
     * \brief A function for restarting robot.
     *
     * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=Controller%20Service
     *
     * \param client RWS client
     * \param mastership {implicit | explicit} by default mastership is explicit
     * \param restartMode Controller restart type
     *
     * \throw \a RWSError if something goes wrong.
     */
    void restartController(RWSClient& client, Mastership const& mastership, RestartMode const& restartMode = RestartMode::restart);
}