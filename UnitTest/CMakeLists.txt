

file(GLOB HEADER *.h) 
file(GLOB SOURCE *.cpp) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${HEADER})
source_group("Source" FILES ${SOURCE})

add_executable(UnitTest ${SOURCE})

target_include_directories(UnitTest
    PRIVATE ../Main/MyLib ../ThirdParty/Catch
)
target_link_libraries(UnitTest 
    MyLib
)

#编译选项
if(MSVC)
  target_compile_options(UnitTest PRIVATE /W3)
endif()