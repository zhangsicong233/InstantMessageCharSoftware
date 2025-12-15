#include "LogicSystem.h"

#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>

#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/ostream.hpp>

#include "HttpConnection.h"
#include "VerifyGrpcClient.h"
#include "const.h"
#include "message.pb.h"

LogicSystem::LogicSystem() {
  RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) {
    boost::beast::ostream(connection->_response.body())
        << "recvive get_test req" << std::endl;

    int cnt = 0;
    for (const auto& elem : connection->_get_params) {
      ++cnt;

      boost::beast::ostream(connection->_response.body())
          << "param " << cnt << "key is " << elem.first;
      boost::beast::ostream(connection->_response.body())
          << ", value is " << elem.second << std::endl;
    }
  });

  RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
    auto body_str =
        boost::beast::buffers_to_string(connection->_request.body().data());
    std::cout << "receive body is " << body_str << std::endl;

    connection->_response.set(boost::beast::http::field::content_type,
                              "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success || !src_root.isMember("email")) {
      std::cout << "Failed to parse JSON data!" << std::endl;

      root["error"] = ErrorCodes::Error_Json;
      std::string jsonstr = root.toStyledString();
      boost::beast::ostream(connection->_response.body()) << jsonstr;

      return true;
    }

    auto email = src_root["email"].asString();
    message::GetVarifyRsp rsp =
        VerifyGrpcClient::GetInstance()->GetVerifyCode(email);
    std::cout << "email is " << email << std::endl;

    root["error"] = rsp.error();
    root["email"] = src_root["email"];
    std::string jsonstr = root.toStyledString();
    boost::beast::ostream(connection->_response.body()) << jsonstr;

    return true;
  });
}

void LogicSystem::RegGet(std::string url, HttpHandler handler) {
  _get_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandleGet(std::string path,
                            std::shared_ptr<HttpConnection> connection) {
  if (_get_handlers.find(path) == _get_handlers.end()) {
    return false;
  }

  _get_handlers[path](connection);

  return true;
}

void LogicSystem::RegPost(std::string url, HttpHandler handler) {
  _post_handlers.insert(std::make_pair(url, handler));
}

bool LogicSystem::HandlePost(std::string path,
                             std::shared_ptr<HttpConnection> connection) {
  if (_post_handlers.find(path) == _post_handlers.end()) {
    return false;
  }

  _post_handlers[path](connection);

  return true;
}

LogicSystem::~LogicSystem() = default;