#include "LogicSystem.h"

#include <boost/beast/core/ostream.hpp>

#include "HttpConnection.h"

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

LogicSystem::~LogicSystem() = default;