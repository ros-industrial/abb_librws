#include <sstream>

#include <Poco/Net/WebSocket.h>

#include <abb_librws/rws_poco_result.h>


using namespace Poco;
using namespace Poco::Net;


namespace abb :: rws
{
  /***********************************************************************************************************************
   * Struct definitions: POCOResult
   */
  POCOResult::POCOResult(Poco::Net::HTTPResponse::HTTPStatus http_status,
    Poco::Net::NameValueCollection const& header_info, std::string const& content)
  : httpStatus_ {http_status}
  , headerInfo_(header_info.begin(), header_info.end())
  , content_ {content}
  {
  };
}