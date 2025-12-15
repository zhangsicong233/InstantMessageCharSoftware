#pragma once

#include "const.h"

class HttpConnection;

using HttpHandler = std::function<void(std::shared_ptr<HttpConnection>)>;

class LogicSystem : public Singleton<LogicSystem> {
  friend Singleton<LogicSystem>;

 public:
  ~LogicSystem();

  void RegGet(std::string url, HttpHandler handler);
  bool HandleGet(std::string, std::shared_ptr<HttpConnection>);

 private:
  LogicSystem();

  std::map<std::string, HttpHandler> _post_handlers;
  std::map<std::string, HttpHandler> _get_handlers;
};