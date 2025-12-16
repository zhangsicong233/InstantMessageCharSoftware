#pragma once

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>

#include "Singleton.hpp"
#include "const.h"
#include "message.grpc.pb.h"

class RPCConPool {
 public:
  RPCConPool(std::size_t poolsize, std::string host, std::string port)
      : _poolSize(poolsize), _host(host), _port(port), _b_stop(false) {
    for (std::size_t i = 0; i < poolsize; ++i) {
      std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
          host + ":" + port, grpc::InsecureChannelCredentials());
      _connections.emplace(message::VarifyService::NewStub(channel));
    }
  }

  std::unique_ptr<message::VarifyService::Stub> getConnection() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock, [this]() {
      if (_b_stop) {
        return true;
      }

      return !_connections.empty();
    });

    if (_b_stop) {
      return nullptr;
    }

    auto context = std::move(_connections.front());
    _connections.pop();

    return context;
  }

  void returnConnection(std::unique_ptr<message::VarifyService::Stub> context) {
    std::unique_lock<std::mutex> lock(_mutex);
    if (_b_stop) {
      return;
    }

    _connections.emplace(std::move(context));
    _cond.notify_one();
  }

  void Close() {
    _b_stop = true;

    _cond.notify_all();
  }

  ~RPCConPool() {
    std::unique_lock<std::mutex> lock(_mutex);

    Close();

    while (!_connections.empty()) {
      _connections.pop();
    }
  }

 private:
  std::size_t _poolSize;
  std::string _host;
  std::string _port;
  std::atomic<bool> _b_stop;

  std::queue<std::unique_ptr<message::VarifyService::Stub>> _connections;

  std::mutex _mutex;
  std::condition_variable _cond;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
  friend class Singleton<VerifyGrpcClient>;

 public:
  message::GetVarifyRsp GetVerifyCode(std::string email);

 private:
  VerifyGrpcClient();

  std::unique_ptr<RPCConPool> _pool;
};