file(GLOB_RECURSE APP_HEADER *.h) 
file(GLOB_RECURSE APP_SOURCE *.cpp *.rc *.def) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

set(Target_Name tinyxmlTest)
add_executable(${Target_Name} ${APP_SOURCE})
