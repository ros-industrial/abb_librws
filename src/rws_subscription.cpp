#include <abb_librws/rws_subscription.h>
#include <abb_librws/rws_error.h>
#include <abb_librws/rws_client.h>
#include <abb_librws/parsing.h>

#include <Poco/Net/HTTPRequest.h>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>


namespace abb :: rws
{
  using namespace Poco::Net;


  SubscriptionGroup::SubscriptionGroup(SubscriptionManager& subscription_manager, SubscriptionResources const& resources)
  : subscription_manager_ {subscription_manager}
  , subscription_group_id_ {subscription_manager.openSubscription(getURI(subscription_manager, resources))}
  {
  }


  SubscriptionGroup::SubscriptionGroup(SubscriptionGroup&& rhs)
  : subscription_manager_ {rhs.subscription_manager_}
  , subscription_group_id_ {rhs.subscription_group_id_}
  {
    // Clear subscription_group_id_ of the SubscriptionGroup that has been moved from,
    // s.t. its destructor does not close the subscription.
    rhs.subscription_group_id_.clear();
  }


  SubscriptionGroup::~SubscriptionGroup()
  {
    try
    {
      // The subscription_group_id_ will be empty if the SubscriptionGroup has been moved from.
      // In this case, we don't have to close the subscription.
      if (!subscription_group_id_.empty())
        subscription_manager_.closeSubscription(subscription_group_id_);
    }
    catch (...)
    {
      // Catch potential exceptions, s.t. the destructor does not throw.
      std::cerr << "Exception in ~SubscriptionGroup(): " << boost::current_exception_diagnostic_information() << std::endl;
    }
  }


  SubscriptionReceiver SubscriptionGroup::receive() const
  {
    return SubscriptionReceiver {subscription_manager_, subscription_group_id_};
  }


  std::vector<std::pair<std::string, SubscriptionPriority>> SubscriptionGroup::getURI(
      SubscriptionManager& subscription_manager, SubscriptionResources const& resources)
  {
    std::vector<std::pair<std::string, SubscriptionPriority>> uri;
    uri.reserve(resources.size());

    for (auto&& r : resources)
      uri.emplace_back(r.getURI(subscription_manager), r.getPriority());

    return uri;
  }


  const std::chrono::microseconds SubscriptionReceiver::DEFAULT_SUBSCRIPTION_TIMEOUT {40000000000};


  SubscriptionReceiver::SubscriptionReceiver(SubscriptionManager& subscription_manager, std::string const& subscription_group_id)
  : subscription_manager_ {subscription_manager}
  , webSocket_ {subscription_manager_.receiveSubscription(subscription_group_id)}
  {
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }


  bool SubscriptionReceiver::waitForEvent(SubscriptionCallback& callback, std::chrono::microseconds timeout)
  {
    WebSocketFrame frame;
    if (webSocketReceiveFrame(frame, timeout))
    {
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);
      subscription_manager_.processEvent(doc, callback);
      return true;
    }

    return false;
  }


  bool SubscriptionReceiver::webSocketReceiveFrame(WebSocketFrame& frame, std::chrono::microseconds timeout)
  {
    auto now = std::chrono::steady_clock::now();
    auto deadline = std::chrono::steady_clock::now() + timeout;

    // If the connection is still active...
    int flags = 0;
    std::string content;
    int number_of_bytes_received = 0;

    // Wait for (non-ping) WebSocket frames.
    do
    {
      now = std::chrono::steady_clock::now();
      if (now >= deadline)
        BOOST_THROW_EXCEPTION(TimeoutError {"WebSocket frame receive timeout"});

      webSocket_.setReceiveTimeout(std::chrono::duration_cast<std::chrono::microseconds>(deadline - now).count());
      flags = 0;

      try
      {
        number_of_bytes_received = webSocket_.receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
      }
      catch (Poco::TimeoutException const&)
      {
        BOOST_THROW_EXCEPTION(
          TimeoutError {"WebSocket frame receive timeout"}
            << boost::errinfo_nested_exception(boost::current_exception())
        );
      }

      content = std::string(websocket_buffer_, number_of_bytes_received);

      // Check for ping frame.
      if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING)
      {
        // Reply with a pong frame.
        webSocket_.sendFrame(websocket_buffer_,
                                number_of_bytes_received,
                                WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
      }
    } while ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING);

    // Check for closing frame.
    if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE)
    {
      // Do not pass content of a closing frame to end user,
      // according to "The WebSocket Protocol" RFC6455.
      frame.frame_content.clear();
      frame.flags = flags;

      return false;
    }

    frame.flags = flags;
    frame.frame_content = content;

    return number_of_bytes_received != 0;
  }


  void SubscriptionReceiver::shutdown()
  {
    // Shut down the socket. This should make webSocketReceiveFrame() return as soon as possible.
    webSocket_.shutdown();
  }


  void SubscriptionCallback::processEvent(IOSignalStateEvent const& event)
  {
  }


  void SubscriptionCallback::processEvent(RAPIDExecutionStateEvent const& event)
  {
  }

}