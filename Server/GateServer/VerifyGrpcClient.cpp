#include "VerifyGrpcClient.h"

#include "ConfigMgr.h"

VerifyGrpcClient::VerifyGrpcClient() {
  auto& gCfgMgr = ConfigMgr::Inst();
  std::string host = gCfgMgr["VarifyServer"]["Host"];
  std::string port = gCfgMgr["VarifyServer"]["Port"];

  _pool.reset(new RPCConPool(5, host, port));
}

message::GetVarifyRsp VerifyGrpcClient::GetVerifyCode(std::string email) {
  grpc::ClientContext context;
  message::GetVarifyRsp reply;
  message::GetVarifyReq request;
  request.set_email(email);

  auto stub = _pool->getConnection();
  grpc::Status status = stub->GetVarifyCode(&context, request, &reply);
  if (status.ok()) {
    _pool->returnConnection(std::move(stub));

    return reply;
  } else {
    _pool->returnConnection(std::move(stub));

    reply.set_error(ErrorCodes::RPCFailed);

    return reply;
  }
}