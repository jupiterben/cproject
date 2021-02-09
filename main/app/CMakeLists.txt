
file(GLOB APP_HEADER *.h) 
file(GLOB APP_SOURCE *.cpp) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

add_executable(app ${APP_SOURCE})

target_include_directories(app
    PRIVATE . ../lib
)
target_link_libraries(app 
    lib
)

#编译选项
if(MSVC)
  target_compile_options(app PRIVATE /W3)
endif()