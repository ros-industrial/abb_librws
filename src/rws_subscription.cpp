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
    return SubscriptionReceiver {subscription_manager_.receiveSubscription(subscription_group_id_)};
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


  const Poco::Timespan SubscriptionReceiver::DEFAULT_SUBSCRIPTION_TIMEOUT {40000000000};


  SubscriptionReceiver::SubscriptionReceiver(Poco::Net::WebSocket&& websocket)
  : webSocket_ {websocket}
  {
    webSocket_.setReceiveTimeout(DEFAULT_SUBSCRIPTION_TIMEOUT);
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }


  bool SubscriptionReceiver::waitForEvent(SubscriptionEvent& event)
  {
    WebSocketFrame frame;
    if (webSocketReceiveFrame(frame))
    {
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);

      event.value = xmlFindTextContent(doc, XMLAttribute {"class", "lvalue"});

      // IMPORTANT: don't use AutoPtr<XML::Node> here! Otherwise you will get memory corruption.
      if (Poco::XML::Node const * node = doc->getNodeByPath("html/body/div/ul/li/a"))
        event.resourceUri = xmlNodeGetAttributeValue(node, "href");

      return true;
    }

    return false;
  }


  bool SubscriptionReceiver::webSocketReceiveFrame(WebSocketFrame& frame)
  {
    // If the connection is still active...
    int flags = 0;
    std::string content;
    int number_of_bytes_received = 0;

    // Wait for (non-ping) WebSocket frames.
    do
    {
      flags = 0;
      number_of_bytes_received = webSocket_.receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
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


  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event)
  {
    return os << "resourceUri=" << event.resourceUri << std::endl
      << "value=" << event.value;
  }
}