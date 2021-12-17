set_property(GLOBAL PROPERTY USE_FOLDERS ON)
file(GLOB_RECURSE SRC_FILES *.h *.cxx *.cpp) 

find_package(ICU REQUIRED COMPONENTS uc data)

add_library(mylib ${SRC_FILES})

target_include_directories(mylib PRIVATE . ${ICU_INCLUDE_DIRS})
target_link_libraries(mylib PRIVATE ${ICU_LIBRARY})

#编译选项
if(MSVC)
  target_compile_options(mylib PRIVATE /W3)
endif()