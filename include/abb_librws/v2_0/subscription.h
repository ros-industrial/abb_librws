#pragma once

#include <abb_librws/v2_0/rws_client.h>

#include <abb_librws/rws_subscription.h>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Element.h>
#include <Poco/Net/WebSocket.h>


namespace abb :: rws :: v2_0 :: subscription
{
  /**
   * \brief Manages an RWS subscription group.
   */
  class SubscriptionGroup
  : public AbstractSubscriptionGroup
  {
  public:
    /**
     * \brief Registers a subscription at the server.
     *
     * \param client RWS client interface
     * \param resources list of resources to subscribe
     */
    explicit SubscriptionGroup(RWSClient& client, SubscriptionResources const& resources);


    /**
     * \a SubscriptionGroup objects are moveable, but not copyable.
     */
    SubscriptionGroup(SubscriptionGroup&&);


    /**
     * \brief Ends an active subscription.
     */
    ~SubscriptionGroup();

    /**
     * \brief Get ID of the subscription group.
     *
     * \return ID of the subscription group.
     */
    std::string const& id() const noexcept override
    {
      return subscription_group_id_;
    }


    /**
     * @brief Get subscribed resources.
     *
     * @return list of subscribed resources.
     */
    SubscriptionResources const& resources() const noexcept override
    {
      return resources_;
    }

    void updateResources(SubscriptionResources const& res) override;
    SubscriptionReceiver receive() const override;


    /**
     * \brief Close the subscription.
     *
     * Closes the subscription as if the destructor was called.
     * The \a id() will become empty and the subsequent calls to \a receive() will fail.
     * This function can be used to close the subscription before destroying the object,
     * and to catch possible errors, which would be problematic with the destructor.
     *
     * \throw \a RWSError if something goes wrong.
     */
    void close() override;


    /**
     * \brief Detach the object from the subscription group.
     *
     * Detaches the object from the actual subscription group, without attempting to close it.
     * The \a id() will become empty and the subsequent calls to \a receive() will fail.
     * This is useful if closing the subscription group fails, but you want to continue.
     */
    void detach() noexcept override;


  private:
    RWSClient& client_;
    SubscriptionResources resources_;

    /**
     * \brief The subscription group id.
     */
    std::string subscription_group_id_;


    /**
     * \brief Subscribe to specified resources.
     *
     * \param client RWS client
     * \param resources list of resources to subscribe
     *
     * \return Id of the created subscription group.
     *
     * \throw \a RWSError if something goes wrong.
     */
    static std::string openSubscription(RWSClient& client, SubscriptionResources const& resources);

    /**
     * \brief End subscription to a specified group.
     *
     * \param client RWS client
     * \param subscription_group_id id of the subscription group to unsubscribe from.
     *
     * \throw \a RWSError if something goes wrong.
     */
    static void closeSubscription(RWSClient& client, std::string const& subscription_group_id);

    static std::string resourcesString(RWSClient& client, SubscriptionResources const& resources);
  };
}