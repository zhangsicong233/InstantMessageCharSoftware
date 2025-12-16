#pragma once

#include <boost/asio.hpp>
#include <vector>

#include "Singleton.hpp"

class AsioIOContextPool : public Singleton<AsioIOContextPool> {
  friend Singleton<AsioIOContextPool>;

 public:
  using IOContext = boost::asio::io_context;
  using WorkGuard = boost::asio::executor_work_guard<IOContext::executor_type>;
  using WorkGuardPtr = std::unique_ptr<WorkGuard>;

  ~AsioIOContextPool();

  AsioIOContextPool(const AsioIOContextPool&) = delete;
  AsioIOContextPool& operator=(const AsioIOContextPool&) = delete;

  // 使用 round-robin 的方式返回一个 io_context
  boost::asio::io_context& GetIOContext();
  void Stop();

 private:
  AsioIOContextPool(std::size_t size = std::thread::hardware_concurrency());

  std::vector<IOContext> _ioContexts;
  std::vector<WorkGuardPtr> _workGuards;
  std::vector<std::thread> _threads;
  std::size_t _nextIOContext;
};
