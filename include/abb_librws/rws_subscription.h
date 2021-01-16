#pragma once

#include "rws_resource.h"
#include "rws_poco_client.h"

#include <Poco/Mutex.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/SharedPtr.h>

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


    class Subscription
    {
    public:
        Subscription();


        /**
         * \brief A method for checking if the WebSocket exist.
         *
         * \return bool flag indicating if the WebSocket exist or not.
         */
        bool webSocketExist() { return !p_websocket_.isNull(); }

        /**
         * \brief A method for connecting a WebSocket.
         *
         * \param uri for the URI (path and query).
         * \param protocol for the WebSocket protocol.
         * \param timeout for the WebSocket communication timeout [microseconds].
         *
         * \return POCOResult containing the result.
         */
        POCOResult webSocketConnect(const std::string& uri, const std::string& protocol, const Poco::Int64 timeout);

        /**
         * \brief A method for receiving a WebSocket frame.
         *
         * \return POCOResult containing the result.
         */
        POCOResult webSocketReceiveFrame();

        /**
         * \brief Forcibly shut down the websocket connection.
         *
         * The connection is shut down immediately.
         * Subsequently, the function will block until a current call to webSocketReceiveFrame() has finished,
         * before cleaning up the local state.
         *
         * Note that since mutexes do not guarantee the order of acquisition for multiple contenders,
         * it is undefined how many calls to webSocketReceiveFrame() will still attempt to use the shut down
         * connection before the local state is cleaned. Those invocation will throw a runtime error.
         */
        void webSocketShutdown();


    private:
        /**
         * \brief A mutex for protecting the client's WebSocket pointer.
         *
         * This mutex must be held while setting or invalidating the p_websocket_ member.
         * Note that the websocket_use_mutex_ must also be held while invalidating the pointer,
         * since someone may be using it otherwise.
         *
         * If acquiring both websocket_connect_mutex_ and websocket_use_mutex_,
         * the connect mutex must be acquired first.
         */
        Poco::Mutex websocket_connect_mutex_;

        /**
         * \brief A mutex for protecting the client's WebSocket resources.
         *
         * This mutex must be held while using the p_websocket_ member,
         * and while invalidating the pointer.
         *
         * If acquiring both websocket_connect_mutex_ and websocket_use_mutex_,
         * the connect mutex must be acquired first.
         */
        Poco::Mutex websocket_use_mutex_;

        /**
         * \brief Static constant for the socket's buffer size.
         */
        static const size_t BUFFER_SIZE = 1024;

        /**
         * \brief A buffer for a WebSocket.
         */
        char websocket_buffer_[BUFFER_SIZE];

        /**
         * \brief A pointer to a WebSocket client.
         */
        Poco::SharedPtr<Poco::Net::WebSocket> p_websocket_;
    };
}