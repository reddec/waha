cmake_minimum_required(VERSION 2.8.4)
project(waha-proxy-balance)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall  -Wextra -pedantic")

set(SOURCE_FILES main.cpp balance.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} zmq zmqpp)
install(TARGETS ${PROJECT_NAME} DESTINATION "local/bin/")