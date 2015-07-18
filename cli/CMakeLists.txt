cmake_minimum_required(VERSION 2.8.4)
project(waha-cli)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

set(SOURCE_FILES main.cpp cli.h ../common/utils.cpp
                readers/reader.h
                readers/args.h readers/args.cpp
                readers/delimited.h readers/delimited.cpp
                readers/json.h readers/json.cpp
                readers/plain.h readers/plain.cpp

                writers/writer.h
                writers/empty.h
                writers/plain.h writers/plain.cpp
                writers/json.h writers/json.cpp
                writers/delimited.h writers/delimited.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} zmq zmqpp jsoncpp PocoFoundation)
install(TARGETS ${PROJECT_NAME} DESTINATION "local/bin/")