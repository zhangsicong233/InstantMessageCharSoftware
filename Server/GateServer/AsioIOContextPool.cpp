#include "AsioIOContextPool.h"

#include <boost/asio/executor_work_guard.hpp>
#include <iostream>

AsioIOContextPool::AsioIOContextPool(std::size_t size)
    : _ioContexts(size), _workGuards(size), _nextIOContext(0) {
  for (std::size_t i = 0; i < size; ++i) {
    _workGuards[i] = std::make_unique<WorkGuard>(_ioContexts[i].get_executor());
  }

  // 遍历多个iocontext，创建多个线程，每个线程内部启动iocontext
  for (std::size_t i = 0; i < _ioContexts.size(); ++i) {
    _threads.emplace_back([this, i]() { _ioContexts[i].run(); });
  }
}

boost::asio::io_context& AsioIOContextPool::GetIOContext() {
  auto& service = _ioContexts[_nextIOContext++];
  _nextIOContext %= _ioContexts.size();

  return service;
}

void AsioIOContextPool::Stop() {
  // 先取消所有 work_guard，让 io_context 能自动退出 run()

  for (auto& guard : _workGuards) {
    guard->reset();  // 新写法，释放 keep-alive
  }

  // 确保 io_context 自己也 stop 掉（防止有其他阻塞任务）
  for (auto& io : _ioContexts) {
    io.stop();
  }

  for (auto& t : _threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

AsioIOContextPool::~AsioIOContextPool() {
  std::cout << "AsioIOContextPool destruct" << std::endl;

  Stop();
}