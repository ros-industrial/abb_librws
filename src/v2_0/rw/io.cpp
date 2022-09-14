#include <abb_librws/v2_0/rw/io.h>
#include <abb_librws/parsing.h>

namespace abb :: rws :: v2_0 :: rw :: io
{
    std::string IOSignalSubscribableResource::getURI() const
    {
      std::string resource_uri = Resources::RW_IOSYSTEM_SIGNALS;
      resource_uri += "/";
      resource_uri += name;
      resource_uri += ";";
      resource_uri += Identifiers::STATE;
      return resource_uri;
    }


    void IOSignalSubscribableResource::processEvent(Poco::XML::Element const& li_element, SubscriptionCallback& callback) const
    {
        if (li_element.getAttribute("class") == "ios-signalstate-ev")
        {
            Poco::XML::Element const * a_element = li_element.getChildElement("a");
            if (!a_element)
                BOOST_THROW_EXCEPTION(ProtocolError {"Cannot parse RWS event message: `li` element has no `a` element"});

            if (a_element->getAttribute("href") == getURI())
            {
                IOSignalStateEvent event;
                event.signal = name;
                event.value = xmlFindTextContent(&li_element, XMLAttribute {"class", "lvalue"});
                event.resource = std::make_shared<IOSignalSubscribableResource>(event.signal);

                callback.processEvent(event);
            }
        }
    }
}