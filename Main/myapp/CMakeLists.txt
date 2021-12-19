set_property(GLOBAL PROPERTY USE_FOLDERS ON)
file(GLOB_RECURSE SRC_FILES *.h *.cxx *.cpp) 

project(myapp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE . ../mylib)
target_link_libraries(${PROJECT_NAME} mylib)

#编译选项
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3)
endif()