#include <sstream>

#include <abb_librws/parsing.h>

#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/DOMParser.h>


namespace abb :: rws
{
  Poco::AutoPtr<Poco::XML::Document> parseXml(std::string const& xml_string)
  {
    static thread_local Poco::XML::DOMParser parser;
    return parser.parseString(xml_string);
  }


  std::vector<Poco::XML::Node*> xmlFindNodes(Poco::XML::Node* p_root, const XMLAttribute& attribute)
  {
    std::vector<Poco::XML::Node*> result;

    if(p_root)
    {
      bool found = false;

      Poco::XML::NodeIterator node_iterator(p_root, Poco::XML::NodeFilter::SHOW_ELEMENT);
      Poco::XML::Node* p_node = node_iterator.nextNode();

      while (p_node)
      {
        if (xmlNodeHasAttribute(p_node, attribute))
        {
          result.push_back(p_node);
        }

        p_node = node_iterator.nextNode();
      }
    }

    return result;
  }

  std::vector<Poco::XML::Node*> xmlFindNodes(Poco::AutoPtr<Poco::XML::Document> p_xml_document,
                                            const XMLAttribute& attribute)
  {
    std::vector<Poco::XML::Node*> result;

    if (!p_xml_document.isNull())
    {
      Poco::XML::NodeIterator node_iterator(p_xml_document, Poco::XML::NodeFilter::SHOW_ELEMENT);
      Poco::XML::Node* p_node = node_iterator.nextNode();

      while (p_node)
      {
        if (xmlNodeHasAttribute(p_node, attribute))
        {
          result.push_back(p_node);
        }

        p_node = node_iterator.nextNode();
      }
    }

    return result;
  }

  std::string xmlFindTextContent(Poco::AutoPtr<Poco::XML::Document> p_xml_document, const XMLAttribute& attribute)
  {
    std::string result;

    if (!p_xml_document.isNull())
    {
      Poco::XML::NodeIterator node_iterator(p_xml_document, Poco::XML::NodeFilter::SHOW_ELEMENT);
      result = xmlFindTextContent(node_iterator.nextNode(), attribute);
    }

    return result;
  }

  std::string xmlFindTextContent(const Poco::XML::Node* p_node, const XMLAttribute& attribute)
  {
    std::string result;

    if (p_node)
    {
      if (p_node->nodeType() == Poco::XML::Node::TEXT_NODE && xmlNodeHasAttribute(p_node->parentNode(), attribute))
      {
        result = p_node->nodeValue();
      }
      else
      {
        bool found = false;
        if (p_node->hasChildNodes())
        {
          Poco::AutoPtr<Poco::XML::NodeList> p_children(p_node->childNodes());

          for (unsigned long i = 0; i < p_children->length() && !found; i++)
          {
            Poco::XML::Node* p_child = p_children->item(i);

            if (p_child->nodeType() == Poco::XML::Node::TEXT_NODE &&
                xmlNodeHasAttribute(p_child->parentNode(), attribute))
            {
              found = true;
              result = p_child->nodeValue();
            }
            else
            {
              result = xmlFindTextContent(p_child, attribute);
              found = !result.empty();
            }
          }
        }
      }
    }

    return result;
  }

  std::string xmlNodeGetAttributeValue(const Poco::XML::Node* p_node, const std::string& name)
  {
    std::string result;

    if (p_node && p_node->hasAttributes() && !name.empty())
    {
      Poco::AutoPtr<Poco::XML::NamedNodeMap> p_attributes(p_node->attributes());

      for (unsigned long i = 0; i < p_attributes->length(); ++i)
      {
        Poco::XML::Node* p_attribute = p_attributes->item(i);

        if (p_attribute->nodeName() == name)
        {
          return p_attribute->nodeValue();
        }
      }
    }

    return result;
  }

  bool xmlNodeHasAttribute(const Poco::XML::Node* p_node, const XMLAttribute& attribute)
  {
    bool found = attribute.name.empty() && attribute.value.empty();

    if(!found && p_node && p_node->hasAttributes())
    {
      Poco::AutoPtr<Poco::XML::NamedNodeMap> p_attributes(p_node->attributes());

      for (unsigned long i = 0; i < p_attributes->length() && !found; i++)
      {
        Poco::XML::Node* p_attribute = p_attributes->item(i);

        if (p_attribute->nodeName() == attribute.name && p_attribute->nodeValue() == attribute.value)
        {
          found = true;
        }
      }
    }

    return found;
  }

  bool xmlNodeHasAttribute(const Poco::XML::Node* p_node, const std::string& name, const std::string& value)
  {
    return xmlNodeHasAttribute(p_node, XMLAttribute(name, value));
  }


  std::string findSubstringContent(const std::string& whole_string,
                                              const std::string& substring_start,
                                              const std::string& substring_end)
  {
    std::string result;
    size_t start_postion = whole_string.find(substring_start);

    if (start_postion != std::string::npos)
    {
      start_postion += substring_start.size();
      size_t end_postion = whole_string.find_first_of(substring_end, start_postion);

      if (end_postion != std::string::npos)
      {
        result = whole_string.substr(start_postion, end_postion - start_postion);
      }
    }

    std::string quot = "&quot;";
    size_t quot_position = 0;
    do
    {
      quot_position = result.find(quot);
      if (quot_position != std::string::npos)
      {
        result.replace(quot_position, quot.size(), "");
      }
    } while (quot_position != std::string::npos);

    return result;
  }

  unsigned int countCharInString(std::string input, const char character)
  {
    bool done = false;
    unsigned int count = 0;
    size_t position = 0;

    do
    {
      position = input.find_first_of(character);

      if (position != std::string::npos)
      {
        ++count;
        input.erase(position, 1);
      }
      else
      {
        done = true;
      }
    } while (!done);

    return count;
  }

  std::vector<std::string> extractDelimitedSubstrings(
      const std::string& input,
      const char start_delimiter,
      const char end_delimiter,
      const char separator
    )
  {
    // Prepare the input by removing any starting and ending '[' respective ']'
    std::string temp_0(input);
    size_t position_1 = 0;
    size_t position_2 = 0;

    position_1 = temp_0.find_first_of(start_delimiter);
    position_2 = temp_0.find_last_of(end_delimiter);

    if (position_1 != std::string::npos && position_2 != std::string::npos)
    {
      temp_0.erase(position_1, 1);
      temp_0.erase(position_2 - 1, 1);
    }

    // Extract and merge the delimited substrings in the prepared input string.
    std::stringstream ss(temp_0);
    std::vector<std::string> values;
    std::string temp_1;
    std::string temp_2;
    int counter = 0;

    while (std::getline(ss, temp_1, separator))
    {
      if (!temp_1.empty())
      {
        counter += countCharInString(temp_1, start_delimiter);
        counter -= countCharInString(temp_1, end_delimiter);

        if (counter == 0)
        {
          if (!temp_2.empty())
          {
            values.push_back(temp_2 + temp_1);
            temp_2 = "";
          }
          else
          {
            values.push_back(temp_1);
          }
        }
        else
        {
          temp_2 += temp_1 + ",";
        }
      }
    }

    return values;
  }
}