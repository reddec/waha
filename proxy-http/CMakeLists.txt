cmake_minimum_required(VERSION 2.8.4)
project(waha-proxy-http)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

set(SOURCE_FILES main.cpp http.cpp http.h ../common/utils.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} zmq zmqpp jsoncpp PocoFoundation PocoNet)
install(TARGETS ${PROJECT_NAME} DESTINATION "local/bin/")