#include <abb_librws/rws_subscription.h>


namespace abb :: rws
{
  using Resources = SystemConstants::RWS::Resources;
  using Identifiers = SystemConstants::RWS::Identifiers;


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


  Subscription::Subscription()
  {

  }


  POCOResult Subscription::webSocketConnect(const std::string& uri,
                                                    const std::string& protocol,
                                                    const Poco::Int64 timeout)
  {
    // Lock the object's mutex. It is released when the method goes out of scope.
    ScopedLock<Mutex> lock(http_mutex_);

    // Result of the communication.
    POCOResult result;

    // The response and the request.
    HTTPResponse response;
    HTTPRequest request(HTTPRequest::HTTP_GET, uri, HTTPRequest::HTTP_1_1);
    request.set("Sec-WebSocket-Protocol", protocol);
    request.setCookies(cookies_);

    // Attempt the communication.
    try
    {
      result.addHTTPRequestInfo(request);

      {
        // We must have at least websocket_connect_mutext_.
        // If a connection already exists, we must also have websocket_use_mutex_.
        // If not, nobody should have the mutex anyway, so we should get it immediately.
        ScopedLock<Mutex> connect_lock(websocket_connect_mutex_);
        ScopedLock<Mutex> use_lock(websocket_use_mutex_);

        p_websocket_ = new WebSocket(http_client_session_, request, response);
        p_websocket_->setReceiveTimeout(Poco::Timespan(timeout));
      }

      result.addHTTPResponseInfo(response);
      result.status = POCOResult::OK;
    }
    catch (InvalidArgumentException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_INVALID_ARGUMENT;
      result.exception_message = e.displayText();
    }
    catch (TimeoutException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_TIMEOUT;
      result.exception_message = e.displayText();
    }
    catch (WebSocketException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_WEBSOCKET;
      result.exception_message = e.displayText();
    }
    catch (NetException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_NET;
      result.exception_message = e.displayText();
    }

    if (result.status != POCOResult::OK)
    {
      http_client_session_.reset();
    }

    return result;
  }

  POCOResult Subscription::webSocketReceiveFrame()
  {
    // Lock the object's mutex. It is released when the method goes out of scope.
    ScopedLock<Mutex> lock(websocket_use_mutex_);

    // Result of the communication.
    POCOResult result;

    // Attempt the communication.
    try
    {
      if (!p_websocket_.isNull())
      {
        int flags = 0;
        std::string content;

        // Wait for (non-ping) WebSocket frames.
        do
        {
          flags = 0;
          int number_of_bytes_received = p_websocket_->receiveFrame(websocket_buffer_, sizeof(websocket_buffer_), flags);
          content = std::string(websocket_buffer_, number_of_bytes_received);

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

          // Shutdown the WebSocket.
          p_websocket_->shutdown();
          p_websocket_ = 0;
        }

        result.addWebSocketFrameInfo(flags, content);
        result.status = POCOResult::OK;
      }
      else
      {
        result.status = POCOResult::WEBSOCKET_NOT_ALLOCATED;
      }
    }
    catch (InvalidArgumentException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_INVALID_ARGUMENT;
      result.exception_message = e.displayText();
    }
    catch (TimeoutException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_TIMEOUT;
      result.exception_message = e.displayText();
    }
    catch (WebSocketException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_WEBSOCKET;
      result.exception_message = e.displayText();
    }
    catch (NetException& e)
    {
      result.status = POCOResult::EXCEPTION_POCO_NET;
      result.exception_message = e.displayText();
    }

    if (result.status != POCOResult::OK)
    {
      http_client_session_.reset();
    }

    return result;
  }

  void Subscription::webSocketShutdown()
  {
    // Make sure nobody is connecting while we're closing.
    ScopedLock<Mutex> connect_lock(websocket_connect_mutex_);

    // Make sure there is actually a connection to close.
    if (!webSocketExist())
    {
      return;
    }

    // Shut down the socket. This should make webSocketReceiveFrame() return as soon as possible.
    p_websocket_->shutdown();

    // Also acquire the websocket lock before invalidating the pointer,
    // or we will break running calls to webSocketReceiveFrame().
    ScopedLock<Mutex> use_lock(websocket_use_mutex_);
    p_websocket_ = Poco::SharedPtr<Poco::Net::WebSocket>();
  }

}