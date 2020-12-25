#pragma once

#include "resource.h"

#include <string>


namespace abb :: rws
{
    /**
     * \brief An enum for specifying subscription priority.
     */
    enum SubscriptionPriority
    {
        LOW,    ///< Low priority.
        MEDIUM, ///< Medium priority.
        HIGH    ///< High priority. Only RobotWare 6.05 (or newer) and for IO signals and persistant RAPID variables.
    };


    /**
     * \brief A struct for containing information about a subscription resource.
     */
    struct SubscriptionResource
    {
        /**
         * \brief URI of the resource.
         */
        std::string resource_uri;

        /**
         * \brief Priority of the subscription.
         */
        SubscriptionPriority priority;

        /**
         * \brief A constructor.
         *
         * \param resource_uri for the URI of the resource.
         * \param priority for the priority of the subscription.
         */
        SubscriptionResource(const std::string& resource_uri, const SubscriptionPriority priority)
        :   resource_uri(resource_uri)
        ,   priority(priority)
        {}
    };


    /**
     * \brief A class for representing subscription resources.
     */
    class SubscriptionResources
    {
    public:
        /**
         * \brief A method to add information about a subscription resource.
         *
         * \param resource_uri for the URI of the resource.
         * \param priority for the priority of the subscription.
         */
        void add(const std::string& resource_uri, const SubscriptionPriority priority);

        /**
         * \brief A method to add information about a IO signal subscription resource.
         *
         * \param iosignal for the IO signal's name.
         * \param priority for the priority of the subscription.
         */
        void addIOSignal(const std::string& iosignal, const SubscriptionPriority priority);

        /**
         * \brief A method to add information about a RAPID persistant symbol subscription resource.
         *
         * \param resource specifying the RAPID task, module and symbol names for the RAPID resource.
         * \param priority for the priority of the subscription.
         */
        void addRAPIDPersistantVariable(const RAPIDResource& resource, const SubscriptionPriority priority);

        /**
         * \brief A method for retrieving the contained subscription resources information.
         *
         * \return std::vector<SubscriptionResource> containing information of the subscription resources.
         */
        const std::vector<SubscriptionResource>& getResources() const { return resources_; }

    private:
        /**
         * \brief A vector of subscription resources.
         */
        std::vector<SubscriptionResource> resources_;
    };
}