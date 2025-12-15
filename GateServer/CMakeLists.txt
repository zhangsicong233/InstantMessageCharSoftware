cmake_minimum_required(VERSION 3.16)
project(GateServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找Boost组件
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# 查找jsoncpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp REQUIRED)

# 添加可执行文件
add_executable(gateserver
    GateServer.cpp
    CServer.cpp
    HttpConnection.cpp
    LogicSystem.cpp
)

# 包含目录
target_include_directories(gateserver PRIVATE .)
target_include_directories(gateserver SYSTEM PRIVATE ${JSONCPP_INCLUDE_DIRS})

# 链接库
target_link_libraries(gateserver PRIVATE Boost::boost Boost::system)
target_link_libraries(gateserver PRIVATE ${JSONCPP_LIBRARIES})

# 线程库
find_package(Threads REQUIRED)
target_link_libraries(gateserver PRIVATE Threads::Threads)

# 定义宏
target_compile_definitions(gateserver PRIVATE BOOST_ASIO_STANDALONE=0)