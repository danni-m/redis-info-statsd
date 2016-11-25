cmake_minimum_required(VERSION 3.6)
project(redis_info_statsd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
find_package (Threads)

find_library (HIREDIS_LIB hiredis PATHS ${REDIS_DIR} )
find_path(HIREDIS_INCLUDE_DIR hiredis)

find_library(RMUTIL_LIb rmutil PATHS RedisModulesSDK/rmutil)

set(SOURCE_FILES main.c parse_info.c parse_info.h statsd-c-client/statsd-client.c)
add_library(redis_info_statsd SHARED ${SOURCE_FILES})
target_link_libraries (redis_info_statsd ${CMAKE_THREAD_LIBS_INIT} ${HIREDIS_LIB} ${RMUTIL_LIb})
include_directories(${HIREDIS_INCLUDE_DIR} RedisModulesSDK)