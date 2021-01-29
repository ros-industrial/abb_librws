#pragma once

#include "rws_resource.h"
#include "rws_client.h"

#include <string>
#include <iosfwd>


namespace abb :: rws
{
  /**
   * \brief An enum for specifying subscription priority.
   */
  enum class SubscriptionPriority
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
    :
    resource_uri(resource_uri),
    priority(priority)
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


  struct SubscriptionEvent
  {
    std::string resourceUri;
    std::string value;
  };


  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event);


  /**
   * \brief Manages RWS event subscription.
   */
  class RWSClient::Subscription
  {
  public:
    using WebSocket = Poco::Net::WebSocket;


    /**
     * \brief A constructor.
     *
     * \param client a client to subscribe
     * \param resources list of resources to subscribe
     */
    Subscription(RWSClient& client, SubscriptionResources const& resources);


    /**
     * \brief Move constructor.
     * 
     * \a Subscription objects are moveable.
     */
    Subscription(Subscription&&) = default;


    /**
     * \brief A destructor.
     */
    ~Subscription();


    /**
     * \brief A method for waiting for a subscription event.
     * 
     * \param event event data
     *
     * \return true if the connection is still alive.
     */
    bool waitForSubscriptionEvent(SubscriptionEvent& event);


    /**
     * \brief A method for ending a active subscription.
     *
     * \return RWSResult containing the result.
     */
    RWSResult endSubscription();


    /**
     * \brief Force close the active subscription connection.
     *
     * This will cause waitForSubscriptionEvent() to return or throw.
     * It does not delete the subscription from the controller.
     *
     * The preferred way to close the subscription is to request the robot controller to end it via
     * endSubscription(). This function can be used to force the connection to close immediately in
     * case the robot controller is not responding.
     *
     * This function blocks until an active waitForSubscriptionEvent() has finished.
     *
     */
    void forceCloseSubscription();


  private:
    RWSClient& client_;


    /**
     * \brief Static constant for the socket's buffer size.
     */
    static const size_t BUFFER_SIZE = 1024;

    /**
     * \brief A buffer for a Subscription.
     */
    char websocket_buffer_[BUFFER_SIZE];

    /**
     * \brief A pointer to a Subscription client.
     */
    std::unique_ptr<WebSocket> p_websocket_;

    /**
     * \brief A subscription group id.
     */
    std::string subscription_group_id_;


    /**
     * \brief Parser for XML in WebSocket frames.
     */
    Poco::XML::DOMParser parser_;


    /**
     * \brief A method for receiving a WebSocket frame.
     * 
     * \brief frame the received frame
     *
     * \return true if the connection is still active, false otherwise.
     */
    bool webSocketReceiveFrame(WebSocketInfo& frame);


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
  };
}