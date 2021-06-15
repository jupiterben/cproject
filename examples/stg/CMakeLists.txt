# 查找文件
file(GLOB_RECURSE APP_HEADER *.h) 
file(GLOB_RECURSE APP_SOURCE *.cpp *.rc *.def) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

#Target
set(target_name stgExample)
add_executable(${target_name} ${APP_SOURCE})

if(MSVC)
    add_definitions(-DNOMINMAX -DUNICODE -D_UNICODE)
endif()