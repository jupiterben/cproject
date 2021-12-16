set_property(GLOBAL PROPERTY USE_FOLDERS ON)
file(GLOB_RECURSE SRC_FILES *.h *.cxx *.cpp) 

add_library(mylib ${SRC_FILES})

target_include_directories(mylib PRIVATE .)

#编译选项
if(MSVC)
  target_compile_options(mylib PRIVATE /W3)
endif()