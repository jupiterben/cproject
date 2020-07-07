

file(GLOB HEADER *.h) 
file(GLOB SOURCE *.cpp) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${HEADER})
source_group("Source" FILES ${SOURCE})

add_library(MyLib ${SOURCE})

target_include_directories(MyLib
   PRIVATE .
)

#编译选项
if(MSVC)
  target_compile_options(MyLib PRIVATE /W3)
endif()