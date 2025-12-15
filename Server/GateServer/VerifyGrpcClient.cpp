#include "VerifyGrpcClient.h"

VerifyGrpcClient::VerifyGrpcClient() {
  std::shared_ptr<grpc::Channel> channel =
      grpc::CreateChannel("0.0.0.50051", grpc::InsecureChannelCredentials());
  _stub = message::VarifyService::NewStub(channel);
}

message::GetVarifyRsp VerifyGrpcClient::GetVerifyCode(std::string email) {
  grpc::ClientContext context;
  message::GetVarifyRsp reply;
  message::GetVarifyReq request;
  request.set_email(email);

  grpc::Status status = _stub->GetVarifyCode(&context, request, &reply);
  if (status.ok()) {
    return reply;
  } else {
    reply.set_error(ErrorCodes::RPCFailed);

    return reply;
  }
}