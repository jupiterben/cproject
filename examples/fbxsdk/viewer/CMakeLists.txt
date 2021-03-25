
file(GLOB_RECURSE APP_HEADER *.h) 
file(GLOB_RECURSE APP_SOURCE *.cpp *.rc *.cxx) 
#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

set(CurrentTarget "FBXViewer")

find_package(FBX)

add_executable(${CurrentTarget} WIN32 ${APP_SOURCE})
target_include_directories(${CurrentTarget} PRIVATE ${FBX_INCLUDE_DIR})
target_link_libraries(${CurrentTarget} PRIVATE ${FBX_LIBRARY})
