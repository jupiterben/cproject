file(GLOB_RECURSE  APP_HEADER *.h) 
file(GLOB_RECURSE  APP_SOURCE *.cpp *.cc) 
#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

find_package(Boost)

project(maxToolTest)

add_executable(${PROJECT_NAME} ${APP_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

#编译选项
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3)
endif()