cmake_minimum_required (VERSION 3.16)

project ("jubeiming-api")

# 设置项目根节点
set(PROJECT_ROOT_NODE ${CMAKE_CURRENT_SOURCE_DIR})

# 输出项目信息
message(STATUS ${PROJECT_NAME} ${PROJECT_VERSION_MAJOR} ${PROJECT_VERSION_MINOR})

# 使用C++20
set(CATKIN_TOPLEVEL TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)


# ===========================================

# 简易三方库设置
set(THIRD_PARTY "${PROJECT_ROOT_NODE}/3rdparty")
include_directories("${THIRD_PARTY}/snowflake-cpp/include") # snowflake库

# bcrypt
set(BCRYPT_PATH "${THIRD_PARTY}/bcrypt-cpp")
include_directories(${BCRYPT_PATH}/include)
# 导入a
link_directories(${BCRYPT_PATH}/lib)

# 三方库版本设置
set(BOOST_VERSION 1.85.0)
set(CONNECT_VERSION 8.4.0)
# 三方库导入相关
# 设置 Boost 库的路径

set(BOOST_COBALT_STATIC_FILE "libboost_cobalt.a")

find_package(Boost REQUIRED COMPONENTS system charconv)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

# redis plus plus
find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)
include_directories(${HIREDIS_HEADER})
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)
include_directories(${REDIS_PLUS_PLUS_HEADER})

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
else ()
    message(FATAL_ERROR "Could not find Boost libraries")
endif ()

find_package(Poco REQUIRED COMPONENTS Util)

find_package(CURL REQUIRED)
find_package(curlpp REQUIRED)
find_package(Crow REQUIRED)
find_package(fmt REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

# 三方库导入相关结束

# ===========================================

# 将指定目录文件所有复制到二进制文件目录
file(GLOB allCopyFiles  "${PROJECT_ROOT_NODE}/resources/*")
file(COPY ${allCopyFiles} DESTINATION ${PROJECT_ROOT_NODE}/bin)

# 指定最终的可执行文件的位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_NODE}/bin)

# 以当前目录为基准
include_directories(${PROJECT_ROOT_NODE}/src)

# 将源代码添加到此项目的可执行文件。
# aux_source_directory(${PROJECT_ROOT_NODE}/src SRC_LIST)
set(MAIN ${PROJECT_ROOT_NODE}/src/main.cpp)

# 设置源文件列表
file(GLOB_RECURSE CPP_FILES "${PROJECT_ROOT_NODE}/src/*.cpp")

# 排除main.cpp
list(REMOVE_ITEM CPP_FILES "${MAIN}")

add_executable(${PROJECT_NAME} ${MAIN})


target_sources(${PROJECT_NAME} PUBLIC ${CPP_FILES})

unset(CPP_FILES)

# 动态库链接
target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow fmt::fmt yaml-cpp spdlog::spdlog nlohmann_json::nlohmann_json
Boost::charconv Boost::system Threads::Threads OpenSSL::Crypto OpenSSL::SSL 
${BOOST_COBALT_STATIC_FILE} ${Poco_LIBRARIES} ${HIREDIS_LIB} ${REDIS_PLUS_PLUS_LIB}
${CURL_LIBRARIES} curlpp libbcrypt.a)

message(G:${CMAKE_C_COMPILER_VERSION},GXX:${CMAKE_CXX_COMPILER_VERSION},C STANDARD:${CMAKE_C_STANDARD},CXX STANDARD:${CMAKE_CXX_STANDARD})
message(final)