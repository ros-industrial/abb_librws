#include <abb_librws/rws.h>
#include <abb_librws/v1_0/rws_client.h>

namespace abb :: rws :: v1_0 :: progress
{
    /**
     * \brief A structure for containing progress resource information.
     */
    struct ProgressInfo
    {
        std::string state;  /** \brief Progress state of resource*/
        int code;           /** \brief Returned retcode*/
    };

    /**
     * \brief Checks progress of provided resource.
     *
     * \param id Resource id to check
     *
     * \return \a ProgressInfo containing the result.
     *
     * \throw \a RWSError if something goes wrong.
     */
    ProgressInfo getProgress(RWSClient &client, std::string const &id);

    /**
     * @brief Structure of progress states of resource.
     */
    struct ProgressState final
    {
        static const std::string READY;     /** Resource is ready. */
        static const std::string PENDING;   /** Resource is beeing processed. */
    };
}
