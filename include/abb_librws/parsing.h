#pragma once

#include "xml_attribute.h"

#include <Poco/DOM/Document.h>
#include <Poco/DOM/AutoPtr.h>

#include <string>
#include <vector>


namespace abb :: rws
{
  /**
   * \brief Returns all children of the given node that have the specifed attribute (name and value).
   *
   * \param p_root for the root node to search.
   * \param attribute specifying the XML attribute (name and value) that the children should have.
   *
   * \return std::vector<Poco::XML::Node*> with the children (empty if none are found, or if the root node is nullptr).
   */
  std::vector<Poco::XML::Node*> xmlFindNodes(Poco::XML::Node* p_root, const XMLAttribute& attribute);

  /**
   * \brief A function for finding all nodes in an XML document that has the specifed attribute (name and value).
   *
   * \param p_xml_document for the XML document to search.
   * \param attribute specifying the XML attribute (name and value) that the XML node should have.
   *
   * \return std::vector<Poco::XML::Node*> containing the found nodes.
   */
  std::vector<Poco::XML::Node*> xmlFindNodes(Poco::AutoPtr<Poco::XML::Document> p_xml_document,
                                          const XMLAttribute& attribute);

  /**
   * \brief A function for finding the text content of an XML node in an XML document. It stops after the first hit.
   *
   * \param p_xml_document for the XML document to search.
   * \param attribute specifying the XML attribute (name and value) that the XML text node should have.
   *
   * \return std::string containing the text content. Empty if none were found.
   */
  std::string xmlFindTextContent(Poco::AutoPtr<Poco::XML::Document> p_xml_document, const XMLAttribute& attribute);

  /**
   * \brief A function for finding the text content of an XML node. If not found, then it checks the node's children.
   *
   * \param p_node for the XML node to search.
   * \param attribute specifying the XML attribute (name and value) that the XML text node (child) should have.
   *
   * \return std::string containing the text content. Empty if none were found.
   */
  std::string xmlFindTextContent(const Poco::XML::Node* p_node, const XMLAttribute& attribute);

  /**
   * \brief A function for getting an XML node's attribute value.
   *
   * \param p_node for the XML node to process.
   * \param name specifying the XML attribute's name to get the value of.
   *
   * \return std::string with the attribute's value. Empty if the attribute was not found.
   */
  std::string xmlNodeGetAttributeValue(const Poco::XML::Node* p_node, const std::string& name);

  /**
   * \brief A function for checking if an XML node has the specified attribute.
   *
   * \param p_node for the XML node to check.
   * \param attribute specifying the XML attribute (name and value) to check for.
   *
   * \return bool indicating if the attribute was found or not.
   */
  bool xmlNodeHasAttribute(const Poco::XML::Node* p_node, const XMLAttribute& attribute);

  /**
   * \brief A function for checking if an XML node has the specified attribute.
   *
   * \param p_node for the XML node to check.
   * \param name specifying the attribute's name.
   * \param value specifying the attribute's value.
   *
   * \return bool indicating if the attribute was found or not.
   */
  bool xmlNodeHasAttribute(const Poco::XML::Node* p_node, const std::string& name, const std::string& value);


  /**
   * \brief A function for retrieving a substring in a string.
   *
   * \param whole_string for the string containing the substring.
   * \param substring_start start of the substring.
   * \param substring_end end of the substring.
   *
   * \return string containing the substring.
   */
  std::string findSubstringContent(const std::string& whole_string,
                                  const std::string& substring_start,
                                  const std::string& substring_end);

  /**
   * \brief A method to count the number of occurrence of a given character in a string.
   *
   * \param input for the string to search.
   * \param character specifying the character to search for.
   *
   * \return unsigned int containing the number of times the character occurs.
   */
  unsigned int countCharInString(std::string input, const char character);

  /**
   * \brief A method to extract delimited substrings in a string.
   *
   * \param input containing the string with delimited substrings.
   * \param start_delimiter delimiter of the start of the string from which to extract the substrings
   * \param end_delimiter delimiter of the end of the string from which to extract the substrings
   * \param separator seperator that is found between 2 substrings that should be extracted
   *
   * \return std::vector<std::string> containing the extracted substrings.
   */
  std::vector<std::string> extractDelimitedSubstrings(const std::string& input, 
                                                     const char start_delimiter,
                                                     const char end_delimiter,
                                                     const char separator);
}