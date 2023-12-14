cmake_minimum_required(VERSION 3.27)
project(ImportScene VERSION 1.0.0 LANGUAGES CXX)

INCLUDE("../../../cmake/Common.cmake")

file(GLOB_RECURSE APP_HEADER *.h)
file(GLOB_RECURSE APP_SOURCE *.cxx "../Common/Common.cxx")

# 在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

add_executable(${PROJECT_NAME} ${APP_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE "D:/Tools/FBX SDK/2020.3.2/include")

set(FBXSDK_ROOT "D:/Tools/FBX SDK/2020.3.2")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FBXSDK_LIB "D:/Tools/FBX SDK/2020.3.2/lib/vs2015/x64/debug")
else()
    set(FBXSDK_LIB "D:/Tools/FBX SDK/2020.3.2/lib/vs2015/x64/release")
endif()

target_link_directories(${PROJECT_NAME} PRIVATE ${FBXSDK_LIB})
target_link_libraries(${PROJECT_NAME} PRIVATE libxml2-md libfbxsdk-md libfbxsdk zlib-md)