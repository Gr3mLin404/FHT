cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(FHT CXX C)
include(ExternalProject)
set (EXECUTABLE_OUTPUT_PATH lib)

if(NOT MSVC)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MINSIZEREL)
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -pthread")
  set(pthread event_pthreads)
endif()
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
  set(LIB_FOR_WIN 
    iphlpapi
    shell32
    advapi32
    ws2_32
    Crypt32)
endif()
find_package(Git REQUIRED)
find_package(OpenSSL REQUIRED)
option(BLOCKING_IO_MODE "Build the blocking io socket" OFF)
if(BLOCKING_IO_MODE)
  message(STATUS "Build the blocking io socket")
  add_compile_definitions(BLOCKING_IO)
endif()
#libevent
ExternalProject_Add(
    libevent
    PREFIX libevent
    GIT_REPOSITORY https://github.com/libevent/libevent.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    BINARY_DIR libevent
    CMAKE_ARGS 
      -DEVENT__LIBRARY_TYPE=STATIC
      -DEVENT__BUILD_SHARED_LIBRARIES=OFF
      -DEVENT__DISABLE_TESTS=ON
      -DEVENT__DISABLE_REGRESS=ON
      -DEVENT__DISABLE_SAMPLES=ON
      -DEVENT__DISABLE_BENCHMARK=ON
      --parallel 16
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)
ExternalProject_Get_property(libevent BINARY_DIR SOURCE_DIR)
set(libevent_BINARY_DIR ${BINARY_DIR})
set(libevent_SOURCE_DIR ${SOURCE_DIR})
link_directories(${libevent_BINARY_DIR}/lib)
set(LibEvent_LIBRARIES 
  event
  event_core
  event_extra
  event_openssl)

#FHT
set(BUILDER_LIB
  src/FHT/lib/base64.cpp
  src/FHT/lib/HostCheckCurl.cpp
)
set(SOURCE_LIB 
  ${BUILDER_LIB}
  src/FHT/Common/Controller/Hendler/Hendler.cpp
  src/FHT/Common/Controller/Logger/Logger.cpp
  src/FHT/Common/Controller/Task/Task.cpp
  src/FHT/Common/Controller/Server/InitialSer.cpp
  src/FHT/Common/Controller/Server/Server.cpp
  src/FHT/Common/Controller/Client/Client.cpp
  src/FHT/Common/Controller/Client/WebClient.cpp
  src/FHT/Common/Controller/Server/WebSocket/User.cpp
  src/FHT/Common/Controller/Server/WebSocket/Frame.cpp
  src/FHT/Common/Controller/Server/WebSocket/WebSocket.cpp
  src/FHT/Common/Controller/Server/WebSocket/Connection.cpp)
set(HEADER_LIB
  src/FHT/lib
  src/FHT/Common/Log
  src/FHT/Interface
)
set(HEADERS 
  ${HEADER_LIB}
  ${OPENSSL_INCLUDE_DIR})
set(HEADER 
  src/FHT/Common
  ${libevent_SOURCE_DIR}/include
  ${libevent_BINARY_DIR}/include)
message(${HEADER})
add_library(fht STATIC ${SOURCE_LIB})
add_dependencies(fht 
  libevent)
target_include_directories(fht PUBLIC ${HEADERS})
target_include_directories(fht PRIVATE ${HEADER})
target_link_libraries(fht
  ${LIB_FOR_WIN}
  ${LibEvent_LIBRARIES}
  ${OPENSSL_LIBRARIES})
set(FHT_LIBRARIES fht)
