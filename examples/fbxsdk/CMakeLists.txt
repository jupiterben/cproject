
file(GLOB APP_HEADER *.h) 
file(GLOB APP_SOURCE *.cpp) 
#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

project(fbxsdkExample)

find_package(FBXSDK CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${APP_SOURCE})
target_link_libraries(${PROJECT_NAME} PRIVATE immer)
