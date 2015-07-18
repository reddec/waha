cmake_minimum_required(VERSION 2.8.4)
project(Waha)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
include_directories(common/)
add_subdirectory(broker)
add_subdirectory(cli)
add_subdirectory(service-script)
add_subdirectory(proxy-balance)
add_subdirectory(proxy-http)

execute_process(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

set(CPACK_GENERATOR "DEB" "ZIP")
set(CPACK_PACKAGE_NAME waha)
set(CPACK_PACKAGE_VERSION 1.0.0)
set(CPACK_PACKAGE_CONTACT "RedDec <net.dev@mail.ru>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ZMQ message router")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${HOST})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libzmq3, libzmqpp3, libpocofoundation9, libpoconet9, libjsoncpp0")
include(CPack)