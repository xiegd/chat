cmake_minimum_required(VERSION 3.26)
set(ProjectName ChatServerTwo)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 17)


# Boost
set(Boost_INCLUDE_DIRS "D:/boost/bin/include/boost-1_84")
set(Boost_LIBRARY_DIRS "D:/boost/msvc_bin_complete/lib")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
set(Boost_LIBRARIES
        libboost_filesystem-vc143-mt-x64-1_84.lib
)

# jsoncpp
set(JsonCpp_ROOT "H:/Programming/C++/jsoncpp")
include_directories(${JsonCpp_ROOT}/include)
link_directories(${JsonCpp_ROOT}/lib)
set(JsonCpp_LIBRARIES
        jsoncpp
)

# gRPC
include("C:/Program Files (x86)/grpc/cmake/common.cmake")
# 定义 Protobuf 文件
set(PROTO_DIR
        ./protos/src
)
set(PROTO_FILES
        ./protos/message.proto
)
set(PROTO_LIB
        message_grpc_proto
)
# 定义文件生成目录
get_filename_component(PROTO_ABS_DIR "${PROTO_DIR}" ABSOLUTE)
# 生成源文件和头文件的列表
set(PROTO_SRC_FILES "")
set(PROTO_HDR_FILES "")
set(GRPC_SRC_FILES "")
set(GRPC_HDR_FILES "")
# 遍历每个 Protobuf 文件
foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_ABS_PATH "${PROTO_FILE}" ABSOLUTE)
    get_filename_component(PROTO_DIR "${PROTO_ABS_PATH}" PATH)
    # 生成源文件和头文件的名称
    get_filename_component(PROTO_NAME "${PROTO_FILE}" NAME_WE)
    set(PROTO_SRC "${PROTO_ABS_DIR}/${PROTO_NAME}.pb.cc")
    set(PROTO_HDR "${PROTO_ABS_DIR}/${PROTO_NAME}.pb.h")
    set(GRPC_SRC "${PROTO_ABS_DIR}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HDR "${PROTO_ABS_DIR}/${PROTO_NAME}.grpc.pb.h")
    # 添加到源文件列表
    list(APPEND PROTO_SRC_FILES "${PROTO_SRC}")
    list(APPEND PROTO_HDR_FILES "${PROTO_HDR}")
    list(APPEND GRPC_SRC_FILES "${GRPC_SRC}")
    list(APPEND GRPC_HDR_FILES "${GRPC_HDR}")
    # 添加自定义命令
    add_custom_command(
            OUTPUT "${PROTO_SRC}" "${PROTO_HDR}" "${GRPC_SRC}" "${GRPC_HDR}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --grpc_out "${PROTO_ABS_DIR}"
            --cpp_out "${PROTO_ABS_DIR}"
            -I "${PROTO_DIR}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${PROTO_ABS_PATH}"
            DEPENDS "${PROTO_ABS_PATH}"
    )
endforeach()
# 创建库
add_library(${PROTO_LIB}
        ${PROTO_SRC_FILES}
        ${PROTO_HDR_FILES}
        ${GRPC_SRC_FILES}
        ${GRPC_HDR_FILES}
)
target_link_libraries(${PROTO_LIB}
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})


# MySQL
set(MySQLConnector_ROOT "C:/Program Files/MySQL/Connector C++ 8.0")
include_directories(${MySQLConnector_ROOT}/include)
link_directories(${MySQLConnector_ROOT}/lib64/vs14)
set(MYSQL_LIBRARIES
        libcrypto.lib
        libssl.lib
        mysqlcppconn.lib
)

# hiredis
set(HIREDIS_ROOT "H:/Programming/C++/hiredis/hiredis")
include_directories(${HIREDIS_ROOT}/include)
link_directories(${HIREDIS_ROOT}/lib)
set(HIREDIS_LIBRARIES
        hiredis_ssl.lib
        hiredis.lib
)



add_executable(${PROJECT_NAME}
        ChatServer/AsioIOService/AsioIOServicePool.h
        ChatServer/ChatGrpc/ChatGrpcClient.h
        ChatServer/Impl/ChatServiceImpl.h
        ChatServer/Config/ConfigMgr.h
        ChatServer/Config/const.h
        ChatServer/CServer/CServer.h
        ChatServer/CServer/CSession.h
        ChatServer/Config/data.h
        ChatServer/CServer/LogicSystem.h
        ChatServer/Message/MsgNode.h
        ChatServer/MySQL/MysqlDao.h
        ChatServer/MySQL/MysqlMgr.h
        ChatServer/Redis/RedisMgr.h
        ChatServer/Base/Singleton.h
        ChatServer/StatusGrpc/StatusGrpcClient.h
        ChatServer/User/UserMgr.h


        ChatServer/AsioIOService/AsioIOServicePool.cpp
        ChatServer/ChatGrpc/ChatGrpcClient.cpp
        ChatServer.cpp
        ChatServer/Impl/ChatServiceImpl.cpp
        ChatServer/Config/ConfigMgr.cpp
        ChatServer/CServer/CServer.cpp
        ChatServer/CServer/CSession.cpp
        ChatServer/CServer/LogicSystem.cpp
        ChatServer/Message/MsgNode.cpp
        ChatServer/MySQL/MysqlDao.cpp
        ChatServer/MySQL/MysqlMgr.cpp
        ChatServer/Redis/RedisMgr.cpp
        ChatServer/StatusGrpc/StatusGrpcClient.cpp
        ChatServer/User/UserMgr.cpp

        protos/src/message.grpc.pb.h
        protos/src/message.pb.h
        protos/src/message.grpc.pb.cc
        protos/src/message.pb.cc
)

target_link_libraries(${ProjectName} ${Boost_LIBRARIES})
target_link_libraries(${ProjectName} ${JsonCpp_LIBRARIES})
target_link_libraries(${PROJECT_NAME}
        ${PROTO_LIB}
        absl::flags
        absl::flags_parse
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
)

target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${HIREDIS_LIBRARIES})