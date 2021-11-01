#pragma once

#include "rws_resource.h"
#include "rws_websocket.h"
#include "rws_error.h"

#include <abb_librws/common/rw/rapid.h>

#include <Poco/DOM/DOMParser.h>
#include <Poco/Net/WebSocket.h>

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <future>
#include <chrono>


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


  class SubscriptionCallback;


  /**
   * \brief Provides the mechanism to open, receive, and close RWS event subscription.
   */
  class SubscriptionManager
  {
  public:
    /**
     * \brief Subscribe to specified resources.
     *
     * \param resources list of pairs (resource URIs, priority) to subscribe
     *
     * \return Id of the created subscription group.
     *
     * \throw \a RWSError if something goes wrong.
     */
    virtual std::string openSubscription(std::vector<std::pair<std::string, SubscriptionPriority>> const& resources) = 0;

    /**
     * \brief End subscription to a specified group.
     *
     * \param subscription_group_id id of the subscription group to unsubscribe from.
     *
     * \throw \a RWSError if something goes wrong.
     */
    virtual void closeSubscription(std::string const& subscription_group_id) = 0;

    /**
     * \brief Open a WebSocket and start receiving subscription events.
     *
     * \param subscription_group_id subscription group id for which to receive event.
     *
     * \return WebSocket created to receive the events.
     *
     * \throw \a RWSError if something goes wrong.
     */
    virtual Poco::Net::WebSocket receiveSubscription(std::string const& subscription_group_id) = 0;

    /**
     * \brief Get URI for subscribing to an IO signal
     *
     * \param io_signal IO signal to subscribe
     *
     * \return Subscription URI for \a io_signal
     */
    virtual std::string getResourceURI(IOSignalResource const& io_signal) const = 0;

    /**
     * \brief Get URI for subscribing to a RAPID variable
     *
     * \param resource RAPID variable resource
     *
     * \return Subscription URI for \a resource
     */
    virtual std::string getResourceURI(RAPIDResource const& resource) const = 0;

    /**
     * \brief Get URI for subscribing to RAPID execution state
     *
     * \return Subscription URI
     */
    virtual std::string getResourceURI(RAPIDExecutionStateResource const&) const = 0;

    /**
     * \brief Process subscription event.
     *
     * Parses the event content \a content, determines event type, and calls the appropriate function in \a callback.
     *
     * \param content XML content of the event
     * \param callback event callback
     */
    virtual void processEvent(Poco::AutoPtr<Poco::XML::Document> content, SubscriptionCallback& callback) const = 0;
  };


  /**
   * \brief Subscription resource that has URI and priority.
   */
  class SubscriptionResource
  {
  public:
    template <typename T>
    SubscriptionResource(T const& resource, SubscriptionPriority priority)
    : resource_ {new ResourceImpl<T> {resource}}
    , priority_ {priority}
    {
    }

    std::string getURI(SubscriptionManager const& subscription_manager) const
    {
      return resource_->getURI(subscription_manager);
    }

    SubscriptionPriority getPriority() const noexcept
    {
      return priority_;
    }

  private:
    struct ResourceInterface
    {
      virtual std::string getURI(SubscriptionManager const& subscription_manager) const = 0;
      virtual ~ResourceInterface() {};
    };

    template <typename T>
    class ResourceImpl
    : public ResourceInterface
    {
    public:
      ResourceImpl(T const& resource)
      : resource_ {resource}
      {
      }

      std::string getURI(SubscriptionManager const& subscription_manager) const override
      {
        return subscription_manager.getResourceURI(resource_);
      }

    private:
      T const resource_;
    };


    std::shared_ptr<ResourceInterface> resource_;

    /**
     * \brief Priority of the subscription.
     */
    SubscriptionPriority priority_;
  };


  using SubscriptionResources = std::vector<SubscriptionResource>;


  /**
   * \brief Event received when an IO signal state changes.
   */
  struct IOSignalStateEvent
  {
    /// \brief IO signal name
    std::string signal;

    /**
     * \brief IO signal value
     */
    std::string value;
  };


  /**
   * \brief Event received when an IO signal state changes.
   */
  struct RAPIDExecutionStateEvent
  {
    /**
     * \brief RAPID execution state
     */
    rw::RAPIDExecutionState state;
  };


  /**
   * \brief Defines callbacks for different types of RWS subscription events.
   */
  class SubscriptionCallback
  {
  public:
    virtual void processEvent(IOSignalStateEvent const& event);
    virtual void processEvent(RAPIDExecutionStateEvent const& event);
  };


  /**
   * \brief Receives RWS subscription events.
   */
  class SubscriptionReceiver
  {
  public:
    /**
     * \brief Prepares to receive events from a specified subscription WebSocket.
     *
     * \param subscription_manager used to initiate WebSocket connection and parse incomoing message content
     * \param subscription_group_id id of the subscription group for which to receive events
     */
    explicit SubscriptionReceiver(SubscriptionManager& subscription_manager, std::string const& subscription_group_id);


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
     * \param callback callback to be called when an event arrives
     * \param timeout wait timeout
     *
     * \return true if the connection is still alive, false if the connection has been closed.
     *
     * \throw \a TimeoutError if waiting time exceeds \a timeout.
     */
    bool waitForEvent(SubscriptionCallback& callback, std::chrono::microseconds timeout = DEFAULT_SUBSCRIPTION_TIMEOUT);


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
    static const std::chrono::microseconds DEFAULT_SUBSCRIPTION_TIMEOUT;

    /**
     * \brief Static constant for the socket's buffer size.
     */
    static const size_t BUFFER_SIZE = 1024;

    /**
     * \brief A buffer for a Subscription.
     */
    char websocket_buffer_[BUFFER_SIZE];

    SubscriptionManager& subscription_manager_;

    /**
     * \brief WebSocket for receiving events.
     */
    Poco::Net::WebSocket webSocket_;

    /**
     * \brief Parser for XML in WebSocket frames.
     */
    Poco::XML::DOMParser parser_;


    bool webSocketReceiveFrame(WebSocketFrame& frame, std::chrono::microseconds timeout);
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
     * \param subscription_manager an interface to control the subscription
     * \param resources list of resources to subscribe
     */
    explicit SubscriptionGroup(SubscriptionManager& subscription_manager, SubscriptionResources const& resources);


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
    void close();


    /**
     * \brief Detach the object from the subscription group.
     *
     * Detaches the object from the actual subscription group, without attempting to close it.
     * The \a id() will become empty and the subsequent calls to \a receive() will fail.
     * This is useful if closing the subscription group fails, but you want to continue.
     */
    void detach() noexcept;


  private:
    static std::vector<std::pair<std::string, SubscriptionPriority>> getURI(
      SubscriptionManager& subscription_manager, SubscriptionResources const& resources);


    SubscriptionManager& subscription_manager_;

    /**
     * \brief A subscription group id.
     */
    std::string subscription_group_id_;
  };


  /**
   * \brief Wait for a subscription event of a specific type.
   *
   * \tparam T type of an event to wait for
   *
   * \param receiver RWS subscription receiver
   * \param timeout wait timeout
   *
   * \return \a std::future with the received event.
   *
   * \throw \a CommunicationError if the subscription WebSocket connection is closed while waiting for the event.
   * \throw \a TimeoutError if waiting time exceeds \a timeout.
   */
  template <typename T>
  inline std::future<T> waitForEvent(SubscriptionReceiver& receiver, std::chrono::microseconds timeout)
  {
    struct Callback : rws::SubscriptionCallback
    {
        void processEvent(T const& event) override
        {
            event_ = event;
        }

        T event_;
    };


    return std::async(std::launch::async,
        [&receiver, timeout] {
            Callback callback;
            if (!receiver.waitForEvent(callback, timeout))
              BOOST_THROW_EXCEPTION(CommunicationError {"WebSocket connection shut down when waiting for a subscription event"});
            return callback.event_;
        }
    );
  }
}