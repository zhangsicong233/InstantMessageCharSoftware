#pragma once

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

#include <memory>

#include "Singleton.hpp"
#include "const.h"
#include "message.grpc.pb.h"
#include "message.pb.h"

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
  friend class Singleton<VerifyGrpcClient>;

 public:
  message::GetVarifyRsp GetVerifyCode(std::string email);

 private:
  VerifyGrpcClient();

  std::unique_ptr<message::VarifyService::Stub> _stub;
};