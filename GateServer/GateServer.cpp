#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/system/detail/error_code.hpp>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "CServer.h"

int main() {
  try {
    boost::asio::io_context ioc{1};

    unsigned short port = static_cast<unsigned short>(8080);
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&ioc](const boost::system::error_code& ec, int signal_number) {
          if (ec) {
            return;
          }

          ioc.stop();
        });

    std::make_shared<CServer>(ioc, port)->Start();

    ioc.run();
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
}