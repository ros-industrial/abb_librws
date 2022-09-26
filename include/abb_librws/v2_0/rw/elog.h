#pragma once

#include <abb_librws/rws.h>
#include <abb_librws/rws_resource.h>
#include <abb_librws/v2_0/rws_client.h>
#include <abb_librws/common/rw/elog.h>

namespace abb ::rws ::v2_0 ::rw ::elog
{
    using namespace rws::rw::elog;

    /**
     * \brief Event log subscription resource
     */
    struct ElogSubscribableResource : public SubscribableResource
    {
        explicit ElogSubscribableResource(int domain);

        std::string getURI() const override;

        void processEvent(Poco::XML::Element const &li_element, SubscriptionCallback &callback) const override;

    private:
        int const domain_;
    };

    /**
     * @brief Get a list of all elog domains.
     * https://developercenter.robotstudio.com/api/rwsApi/elog_resources_page.html
     *
     * @param client RWSClient
     * @param lang The language in which domain names will be returned. English default.
     * @return std::vector<std::pair<int, std::string>> - List of domain id and domain name in provided language
     */
    std::vector<std::pair<int, std::string>> getElogDomains(RWSClient &client, std::string lang = Language::ENGLISH);

    /**
     * @brief Get a specific elog message in the given domain.
     * https://developercenter.robotstudio.com/api/rwsApi/elog_domain_get_page.html
     *
     * @param client RWSClient
     * @param domain domain number
     * @param seqnum sequence number
     * @param lang The language in which messages will be returned. English default.
     * @return ElogMessage structure
     */
    ElogMessage getElogMessage(RWSClient &client, int const domain, int const seqnum, std::string const &lang = Language::ENGLISH);
}
