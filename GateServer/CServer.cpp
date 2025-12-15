#include "CServer.h"

#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short port)
    : _ioc(ioc),
      _socket(ioc),
      _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                                    port)) {}

void CServer::Start() {
  auto self = shared_from_this();
  _acceptor.async_accept(_socket, [self](boost::beast::error_code ec) {
    try {
      if (ec) {
        // 出错放弃该链接， 继续监听其他链接
        self->Start();

        return;
      }

      // 创建新连接，并创建 HttpConnection 类管理这个连接
      std::make_shared<HttpConnection>(std::move(self->_socket))->Start();

      // 继续监听
      self->Start();
    } catch (std::exception& exp) {
      std::cout << "exception is " << exp.what() << std::endl;

      self->Start();
    }
  });
}