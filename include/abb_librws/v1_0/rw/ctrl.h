
#include <abb_librws/rws.h>
#include <abb_librws/v1_0/rws_client.h>
#include <abb_librws/common/rw/ctrl.h>

#include <string>
#include <iosfwd>

namespace abb :: rws :: v1_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v1_0 :: rw :: ctrl
{
    /**
     * \brief A function for restarting robot.
     *
     * https://developercenter.robotstudio.com/api/RWS?urls.primaryName=Controller%20Service
     *
     * \param client RWS client
     * \param restartMode Controller restart type
     *
     * \throw \a RWSError if something goes wrong.
     */
    void restartController(RWSClient& client, RestartMode const& restartMode = RestartMode::restart);

}