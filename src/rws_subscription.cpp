#include <abb_librws/rws_subscription.h>

#include <iostream>
#include <sstream>


namespace abb :: rws
{
  using namespace Poco::Net;


  typedef SystemConstants::RWS::Resources     Resources;
  typedef SystemConstants::RWS::Identifiers   Identifiers;
  typedef SystemConstants::RWS::Services      Services;


  /***********************************************************************************************************************
   * Class definitions: SubscriptionResources
   */

  /************************************************************
   * Primary methods
   */

  void SubscriptionResources::addIOSignal(const std::string& iosignal, const SubscriptionPriority priority)
  {
    std::string resource_uri = Resources::RW_IOSYSTEM_SIGNALS;
    resource_uri += "/";
    resource_uri += iosignal;
    resource_uri += ";";
    resource_uri += Identifiers::STATE;

    add(resource_uri, priority);
  }

  void SubscriptionResources::addRAPIDPersistantVariable(const RAPIDResource& resource, const SubscriptionPriority priority)
  {
    std::string resource_uri = Resources::RW_RAPID_SYMBOL_DATA_RAPID;
    resource_uri += "/";
    resource_uri += resource.task;
    resource_uri += "/";
    resource_uri += resource.module;
    resource_uri += "/";
    resource_uri += resource.name;
    resource_uri += ";";
    resource_uri += Identifiers::VALUE;

    add(resource_uri, priority);
  }

  void SubscriptionResources::add(const std::string& resource_uri, const SubscriptionPriority priority)
  {
    resources_.push_back(SubscriptionResource(resource_uri, priority));
  }


  SubscriptionGroup::SubscriptionGroup(POCOClient& client, SubscriptionResources const& resources)
  : client_ {client}
  {
    std::vector<SubscriptionResource> temp = resources.getResources();

    // Generate content for a subscription HTTP post request.
    std::stringstream subscription_content;
    for (std::size_t i = 0; i < temp.size(); ++i)
    {
      subscription_content << "resources=" << i
                            << "&"
                            << i << "=" << temp.at(i).resource_uri
                            << "&"
                            << i << "-p=" << static_cast<int>(temp.at(i).priority)
                            << (i < temp.size() - 1 ? "&" : "");
    }

    // Make a subscription request.
    POCOResult const poco_result = client_.httpPost(Services::SUBSCRIPTION, subscription_content.str());

    if (poco_result.poco_info.http.response.status != HTTPResponse::HTTP_CREATED)
      throw std::runtime_error("Unable to create Subscription: " + poco_result.poco_info.http.response.content);

    subscription_group_id_ = findSubstringContent(poco_result.poco_info.http.response.header_info, "/poll/", "\n");
  }


  SubscriptionGroup::~SubscriptionGroup()
  {
    // Unsubscribe from events
    std::string const uri = Services::SUBSCRIPTION + "/" + subscription_group_id_;
    client_.httpDelete(uri);
  }


  SubscriptionReceiver::SubscriptionReceiver(POCOClient& client, std::string const& subscription_group_id)
  : p_websocket_ {client.webSocketConnect("/poll/" + subscription_group_id, "robapi2_subscription", DEFAULT_SUBSCRIPTION_TIMEOUT)}
  {
  }


  SubscriptionReceiver::~SubscriptionReceiver()
  {
  }
  
  
  bool SubscriptionReceiver::waitForEvent(SubscriptionEvent& event)
  {
    WebSocketInfo frame;
    if (webSocketReceiveFrame(frame))
    {
      // std::clog << "WebSocket frame received: flags=" << frame.flags << ", frame_content=" << frame.frame_content << std::endl;
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);

      event.value = xmlFindTextContent(doc, XMLAttribute {"class", "lvalue"});

      // IMPORTANT: don't use AutoPtr<XML::Node> here! Otherwise you will get memory corruption.
      if (Poco::XML::Node const * node = doc->getNodeByPath("html/body/div/ul/li/a"))
        event.resourceUri = xmlNodeGetAttributeValue(node, "href");

      return true;
    }
    
    return false;
  }


  bool SubscriptionReceiver::webSocketReceiveFrame(WebSocketInfo& frame)
  {
    // Lock the object's mutex. It is released when the method goes out of scope.
    std::lock_guard<std::mutex> lock {websocket_use_mutex_};

    // If the connection is still active...
    if (p_websocket_)
    {
      int flags = 0;
      std::string content;

      // Wait for (non-ping) WebSocket frames.
      do
      {
        flags = 0;
        int number_of_bytes_received = p_websocket_->receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);

        // std::clog << "Subscription::webSocketReceiveFrame: " << number_of_bytes_received << " bytes received\n";
        content = std::string(websocket_buffer_, number_of_bytes_received);
        // std::clog << "WebSocket frame received: flags=" << flags << ", content=" << content << std::endl;

        // Check for ping frame.
        if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING)
        {
          // Reply with a pong frame.
          p_websocket_->sendFrame(websocket_buffer_,
                                  number_of_bytes_received,
                                  WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
        }
      } while ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING);

      // Check for closing frame.
      if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE)
      {
        // Do not pass content of a closing frame to end user,
        // according to "The WebSocket Protocol" RFC6455.
        content.clear();

        // Destroy the WebSocket to indicate that the connection is shut down.
        p_websocket_.reset();
      }

      frame.flags = flags;
      frame.frame_content = content;
    }

    return (bool)p_websocket_;
  }


  void SubscriptionReceiver::forceClose()
  {
    webSocketShutdown();
  }


  void SubscriptionReceiver::webSocketShutdown()
  {
    if (p_websocket_)
    {
      // Shut down the socket. This should make webSocketReceiveFrame() return as soon as possible.
      p_websocket_->shutdown();

      // Also acquire the websocket lock before invalidating the pointer,
      // or we will break running calls to webSocketReceiveFrame().
      std::lock_guard<std::mutex> use_lock(websocket_use_mutex_);
      p_websocket_.reset();
    }
  }


  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event)
  {
    return os << "resourceUri=" << event.resourceUri << std::endl
      << "value=" << event.value;
  }
}