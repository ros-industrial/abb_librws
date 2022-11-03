#include <abb_librws/rws_subscription.h>
#include <abb_librws/rws_error.h>
#include <abb_librws/parsing.h>

#include <Poco/Net/HTTPRequest.h>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>


namespace abb :: rws
{
  using namespace Poco::Net;


  const std::chrono::microseconds SubscriptionReceiver::DEFAULT_SUBSCRIPTION_NEW_MESSAGE_TIMEOUT = std::chrono::hours {48};  // 2 days

  // 60 seconds should work for both for RWS1 and RWS2, but since on RWS2 TLS is used and due to a bug in POCO
  // (https://github.com/pocoproject/poco/issues/3848), the actual timeout will be 120 seconds.
  const std::chrono::microseconds SubscriptionReceiver::DEFAULT_SUBSCRIPTION_PING_PONG_TIMEOUT = std::chrono::seconds {60};


  SubscriptionReceiver::SubscriptionReceiver(SubscriptionManager& subscription_manager, AbstractSubscriptionGroup const& group)
  : group_ {group}
  , subscription_manager_ {subscription_manager}
  , webSocket_ {subscription_manager_.receiveSubscription(group.id())}
  {
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }


  bool SubscriptionReceiver::waitForEvent(SubscriptionCallback& callback,
                                                    std::chrono::microseconds ping_pong_timeout,
                                                    std::chrono::microseconds new_message_timeout)
  {
    WebSocketFrame frame;
    if (webSocketReceiveFrame(frame, ping_pong_timeout, new_message_timeout))
    {
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);
      processAllEvents(doc, group_.resources(), callback);
      return true;
    }

    return false;
  }


  bool SubscriptionReceiver::webSocketReceiveFrame(WebSocketFrame& frame,
                                                    std::chrono::microseconds ping_pong_timeout,
                                                    std::chrono::microseconds new_message_timeout)
  {
    auto now = std::chrono::steady_clock::now();
    auto deadline = std::chrono::steady_clock::now() + new_message_timeout;

    // If the connection is still active...
    int flags = 0;
    std::string content;
    int number_of_bytes_received = 0;

    // Wait for (non-ping) WebSocket frames.
    do
    {
      now = std::chrono::steady_clock::now();
      if (now >= deadline)
        BOOST_THROW_EXCEPTION(TimeoutError {"WebSocket Failed to receive new subscription message in " +
                                            std::to_string(new_message_timeout.count()) + " microseconds."});

      // Set the timeout for the next receive operation (we receive ping-pong messages every 30 seconds from controller).
      // If the timeout is larger than deadline, we set it to deadline.
      auto current_new_message_timeout = std::chrono::duration_cast<std::chrono::microseconds>(deadline-now);
      bool wait_on_deadline = current_new_message_timeout < ping_pong_timeout;

      Poco::Timespan timeout {wait_on_deadline ? current_new_message_timeout.count() : ping_pong_timeout.count()};

      webSocket_.setReceiveTimeout(timeout);
      flags = 0;

      try
      {
        number_of_bytes_received = webSocket_.receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
      }
      catch (Poco::TimeoutException const&)
      {
        if (wait_on_deadline)
            BOOST_THROW_EXCEPTION(TimeoutError {"WebSocket Failed to receive new subscription message in " +
                                            std::to_string(new_message_timeout.count()) + " microseconds."});

        BOOST_THROW_EXCEPTION(TimeoutError {"WebSocket Failed to receive ping-pong message in " +
                                            std::to_string(ping_pong_timeout.count()) + " microseconds."});
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

  void processAllEvents(Poco::AutoPtr<Poco::XML::Document> doc, SubscriptionResources const& resources, SubscriptionCallback& callback)
  {
    // IMPORTANT: don't use AutoPtr<XML::Element> here! Otherwise you will get memory corruption.
    Poco::XML::Element const * ul_element = dynamic_cast<Poco::XML::Element const *>(doc->getNodeByPath("html/body/div/ul"));
    if (!ul_element)
      BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: can't find XML element at path html/body/div/ul"});

    // Cycle through all <li> elements
    Poco::AutoPtr<Poco::XML::NodeList> li_elements = ul_element->getElementsByTagName("li");
    for (unsigned long index = 0; index < li_elements->length(); ++index)
    {
      Poco::XML::Element const * li_element = dynamic_cast<Poco::XML::Element const *>(li_elements->item(index));
      if (!li_element)
        BOOST_THROW_EXCEPTION(std::logic_error {"An item of the list returned by getElementsByTagName() is not an XML::Element"});

      // Cycle throught all subscription resources
      for (auto const& resource : resources)
        resource.processEvent(*li_element, callback);
    }
  }
}