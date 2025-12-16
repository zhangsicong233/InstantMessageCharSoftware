#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
  friend class LogicSystem;

 public:
  HttpConnection(boost::asio::io_context& ioc);

  void Start();
  boost::asio::ip::tcp::socket& GetSocket();

 private:
  boost::asio::ip::tcp::socket _socket;

  boost::beast::flat_buffer _buffer{8192};
  boost::beast::http::request<boost::beast::http::dynamic_body> _request;
  boost::beast::http::response<boost::beast::http::dynamic_body> _response;

  boost::asio::steady_timer _deadline{_socket.get_executor(),
                                      std::chrono::seconds(60)};

  std::string _get_url;
  std::unordered_map<std::string, std::string> _get_params;

  void CheckDeadline();
  void WriteResponse();
  void HandleReq();
  void PreParseGetParam();
};