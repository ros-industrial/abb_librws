#pragma once

#include "rws_resource.h"
#include "rws_poco_client.h"
#include "rws_websocket.h"

#include <Poco/DOM/DOMParser.h>

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


  /**
   * \brief Contains info about a subscribed event.
   */
  struct SubscriptionEvent
  {
    /**
     * \brief URI of the subscribed resource
     */
    std::string resourceUri;

    /**
     * \brief Value associated with the resource
     */
    std::string value;
  };


  /**
   * \brief Outputs a \a SubscriptionEvent in a human-readable format.
   */
  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event);


  /**
   * \brief Receives RWS subscription events.
   */
  class SubscriptionReceiver
  {
  public:
    /**
     * \brief Prepares to receive events from a specified subscription WebSocket.
     *
     * Normally you don't want to construct \a SubscriptionReceiver manually.
     * Use \a SubscriptionGroup::receive() for this.
     *
     * \param websocket A connected WebSocket that should be used to receive events.
     */
    explicit SubscriptionReceiver(Poco::Net::WebSocket&& websocket);


    /**
     * \brief \a SubscriptionReceiver objects are moveable, but not copyable.
     */
    SubscriptionReceiver(SubscriptionReceiver&&) = default;


    /**
     * \brief Closes the WebSocket connection but does not delete the subscription.
     */
    ~SubscriptionReceiver();


    /**
     * \brief Waits for a subscription event.
     *
     * \param event event data
     *
     * \return true if the connection is still alive, false if the connection has been closed.
     */
    bool waitForEvent(SubscriptionEvent& event);


    /**
     * \brief Shutdown the active subscription connection.
     *
     * If waitForEvent() is being executed on a different thread, it will return or throw.
     * It does not delete the subscription from the controller.
     *
     * The preferred way to close the subscription is the destruction of the \a SubscriptionGroup object.
     * This function can be used to force the connection to close immediately in
     * case the robot controller is not responding.
     *
     * This function will return immediately and does not block until an active waitForEvent() has finished.
     *
     */
    void shutdown();


  private:
    /**
     * \brief Default RWS subscription timeout [microseconds].
     */
    static const Poco::Timespan DEFAULT_SUBSCRIPTION_TIMEOUT;

    /**
     * \brief Static constant for the socket's buffer size.
     */
    static const size_t BUFFER_SIZE = 1024;

    /**
     * \brief A buffer for a Subscription.
     */
    char websocket_buffer_[BUFFER_SIZE];

    /**
     * \brief WebSocket for receiving events.
     */
    Poco::Net::WebSocket webSocket_;

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
    bool webSocketReceiveFrame(WebSocketFrame& frame);
  };


  /**
   * \brief Manages an RWS subscription group.
   */
  class SubscriptionGroup
  {
  public:
    /**
     * \brief Registers a subscription at the server.
     *
     * \param client a client to subscribe. The lifetime of the client must exceed the lifetime of the subscription group.
     * \param resources list of resources to subscribe
     */
    SubscriptionGroup(POCOClient& client, SubscriptionResources const& resources);


    /**
     * \a SubscriptionGroup objects are moveable, but not copyable.
     */
    SubscriptionGroup(SubscriptionGroup&&) = default;


    /**
     * \brief Ends an active subscription.
     */
    ~SubscriptionGroup();

    /**
     * \brief Get ID of the subscription group.
     *
     * \return ID of the subscription group.
     */
    std::string const& id() const noexcept
    {
      return subscription_group_id_;
    }


    /**
     * \brief Establish WebSocket connection ans start receiving subscription events.
     *
     * \return \a SubscriptionReceiver object that can be used to receive events.
     */
    SubscriptionReceiver receive() const;


  private:
    POCOClient& client_;

    /**
     * \brief A subscription group id.
     */
    std::string subscription_group_id_;
  };
}