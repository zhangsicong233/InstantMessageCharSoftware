cmake_minimum_required(VERSION 3.16)
project(GateServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找Boost组件
find_package(Boost 1.70 REQUIRED COMPONENTS system filesystem)

# 查找Protobuf和gRPC
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 查找jsoncpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp REQUIRED)
# 查找hiredis
pkg_check_modules(HIREDIS hiredis REQUIRED)

# 添加可执行文件
add_executable(gateserver
    GateServer.cpp
    CServer.cpp
    HttpConnection.cpp
    LogicSystem.cpp
    ConfigMgr.cpp
    VerifyGrpcClient.cpp
    AsioIOContextPool.cpp
    RedisMgr.cpp
    message.grpc.pb.cc
    message.pb.cc
)

# 包含目录
target_include_directories(gateserver PRIVATE .)
target_include_directories(gateserver SYSTEM PRIVATE ${JSONCPP_INCLUDE_DIRS})
target_include_directories(gateserver SYSTEM PRIVATE ${HIREDIS_INCLUDE_DIRS})
target_include_directories(gateserver SYSTEM PRIVATE ${Protobuf_INCLUDE_DIRS})
target_include_directories(gateserver SYSTEM PRIVATE ${gRPC_INCLUDE_DIRS})

# 链接库
target_link_libraries(gateserver PRIVATE Boost::boost Boost::system Boost::filesystem)
target_link_libraries(gateserver PRIVATE ${JSONCPP_LIBRARIES})
target_link_libraries(gateserver PRIVATE ${HIREDIS_LIBRARIES})
target_link_libraries(gateserver PRIVATE ${Protobuf_LIBRARIES})
target_link_libraries(gateserver PRIVATE gRPC::grpc gRPC::grpc++)

# 线程库
find_package(Threads REQUIRED)
target_link_libraries(gateserver PRIVATE Threads::Threads)

# 定义宏
target_compile_definitions(gateserver PRIVATE BOOST_ASIO_STANDALONE=0)

# 复制配置文件到构建目录
configure_file(config.ini config.ini COPYONLY)