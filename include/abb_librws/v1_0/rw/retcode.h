#pragma once

#include <abb_librws/rws.h>
#include <abb_librws/common/rw/retcode.h>
#include <abb_librws/v1_0/rws_client.h>

#include <string>
#include <map>

namespace abb::rws::v1_0::rw::retcode
{
    using namespace abb::rws::rw::retcode;

    /**
     * \brief Get description of diagnostic code.
     *
     * https://developercenter.robotstudio.com/api/rwsApi/rwretcode_get_return_codes.html
     *
     * \param client RWS client
     * \param code code to be checked.
     *
     * \return \a RetcodeInfo containing the result.
     *
     * \throw \a RWSError if something goes wrong.
     */
    RetcodeInfo getRetcodeInfo(RWSClient &client, int code);

    /**
     * @brief Get descriptions of all diagnostic codes.
     *
     * https://developercenter.robotstudio.com/api/rwsApi/rwretcode_get_return_codes.html
     *
     * \param client RWS client
     *
     * \return \a map<int,RetcodeInfo> with error code as key and RetcodeInfo structure as value.
     *
     * \throw \a RWSError if something goes wrong.
     */
    std::map<int, RetcodeInfo> getRetcodes(RWSClient &client);
}