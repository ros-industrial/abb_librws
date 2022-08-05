#include <abb_librws/rws.h>
#include <abb_librws/v2_0/rws_client.h>

#include <string>


namespace abb :: rws :: v2_0 :: rw
{
    using namespace rws::rw;
}


namespace abb :: rws :: v2_0 :: rw :: retcode
{
    /**
     * \brief A structure for containing response of recode resource.
     */
    struct RetcodeInfo
    {
        int code;                   /** \brief The error code or success code as number */
        std::string name;           /** \brief The name of error code or success code */
        std::string severity;       /** \brief Success, Warning or Error */
        std::string description;    /** \brief Short description of the error code */
    };


    /**
     * \brief A function for checking description of diagnostic code.
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
    RetcodeInfo getRetcodeInfo(RWSClient& client, int code);
}
