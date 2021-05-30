
file(GLOB_RECURSE APP_HEADER *.h) 
file(GLOB_RECURSE APP_SOURCE *.cpp) 
#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

set(CurrentTarget "FBXSample")

find_package(FBX)
find_package(rapidjson CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(${CurrentTarget} ${APP_SOURCE})
target_include_directories(${CurrentTarget} PRIVATE ${FBX_INCLUDE_DIR} ${RapidJSON_INCLUDE_DIRS} )
target_link_libraries(${CurrentTarget} PRIVATE ${FBX_LIBRARY} nlohmann_json nlohmann_json::nlohmann_json)
