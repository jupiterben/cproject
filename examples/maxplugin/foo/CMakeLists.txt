file(GLOB_RECURSE APP_HEADER *.h) 
file(GLOB_RECURSE APP_SOURCE *.cpp *.rc *.def) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

project(maxplugin)

add_definitions(-DUNICODE -D_UNICODE)

find_package(3DSMAX)

set(target_Name maxPlugin)
add_library(${target_Name} SHARED ${APP_SOURCE})
target_include_directories(${target_Name} PRIVATE ${3DSMAX_INCLUDE_DIR})
target_link_directories(${target_Name} PRIVATE ${3DSMAX_LIB_DIR})
target_link_libraries(${target_Name} PRIVATE ${3DSMAX_LIBRARIES})
set_target_properties(${target_Name} PROPERTIES SUFFIX ".gup")