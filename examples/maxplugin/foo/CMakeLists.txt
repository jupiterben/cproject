file(GLOB_RECURSE APP_HEADER *.h) 
file(GLOB_RECURSE APP_SOURCE *.cpp *.rc *.def) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

project(maxplugin)

add_definitions(-DUNICODE -D_UNICODE)

find_package(3DSMAX)

set(Target_Name maxPlugin)
add_library(${Target_Name} SHARED ${APP_SOURCE})
target_include_directories(${Target_Name} PRIVATE ${3DSMAX_INCLUDE_DIR})
target_link_directories(${Target_Name} PRIVATE ${3DSMAX_LIB_DIR})
target_link_libraries(${Target_Name} PRIVATE ${3DSMAX_LIBRARIES})
set_target_properties(${Target_Name} PROPERTIES SUFFIX ".gup")