#include <abb_librws/rws_subscription.h>

#include <iostream>


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


  RWSClient::Subscription::Subscription(RWSClient& client, SubscriptionResources const& resources)
  : client_ {client}
  {
    RWSResult result;

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
    EvaluationConditions evaluation_conditions;
    evaluation_conditions.parse_message_into_xml = false;
    evaluation_conditions.accepted_outcomes.push_back(HTTPResponse::HTTP_CREATED);
    POCOResult poco_result = client_.httpPost(Services::SUBSCRIPTION, subscription_content.str());
    result = client_.evaluatePOCOResult(poco_result, evaluation_conditions);

    if (!result.success)
      throw std::runtime_error("Unable to create RWSClient::Subscription: " + result.error_message);

    std::string poll = "/poll/";
    subscription_group_id_ = findSubstringContent(poco_result.poco_info.http.response.header_info, poll, "\n");
    poll += subscription_group_id_;

    p_websocket_ = client_.webSocketConnect(poll, "robapi2_subscription", DEFAULT_SUBSCRIPTION_TIMEOUT);
  }



  RWSClient::Subscription::~Subscription()
  {
    try
    {
      // Unsubscribe from events
      endSubscription();
    }
    catch (...)
    {
      // Catch all exceptions in dtor
    }
  }


  bool RWSClient::Subscription::waitForSubscriptionEvent(SubscriptionEvent& event)
  {
    WebSocketInfo frame;
    if (webSocketReceiveFrame(frame))
    {
      // std::clog << "WebSocket frame received: flags=" << frame.flags << ", frame_content=" << frame.frame_content << std::endl;
      Poco::AutoPtr<Poco::XML::Document> doc = parser_.parseString(frame.frame_content);

      event.value = xmlFindTextContent(doc, XMLAttribute {"class", "lvalue"});
      if (Poco::AutoPtr<Poco::XML::Node> node = doc->getNodeByPath("html/body/div/ul/li/a"))
        event.resourceUri = xmlNodeGetAttributeValue(node, "href");

      return true;
    }
    
    return false;
  }


  RWSResult RWSClient::Subscription::endSubscription()
  {
    RWSResult result;

    std::string uri = Services::SUBSCRIPTION + "/" + subscription_group_id_;

    EvaluationConditions evaluation_conditions;
    evaluation_conditions.parse_message_into_xml = false;
    evaluation_conditions.accepted_outcomes.push_back(HTTPResponse::HTTP_OK);

    result = client_.evaluatePOCOResult(client_.httpDelete(uri), evaluation_conditions);

    return result;
  }


  bool RWSClient::Subscription::webSocketReceiveFrame(WebSocketInfo& frame)
  {
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

      frame.flags = flags;
      frame.frame_content = content;

      // Check for closing frame.
      if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE)
      {
        // Do not pass content of a closing frame to end user,
        // according to "The WebSocket Protocol" RFC6455.
        content.clear();

        // Shutdown the WebSocket.
        p_websocket_->shutdown();

        // Destroy the WebSocket.
      }
    }

    return (bool)p_websocket_;
  }


  void RWSClient::Subscription::webSocketShutdown()
  {
    if (p_websocket_)
      // Shut down the socket. This should make webSocketReceiveFrame() return as soon as possible.
      p_websocket_->shutdown();
  }


  std::ostream& operator<<(std::ostream& os, SubscriptionEvent const& event)
  {
    return os << "resourceUri=" << event.resourceUri << std::endl
      << "value=" << event.value;
  }
}