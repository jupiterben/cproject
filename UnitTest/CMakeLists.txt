

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


if(EMSCRIPTEN)
    set_target_properties(UnitTest PROPERTIES LINK_FLAGS "-s SINGLE_FILE=1 -s USE_FREETYPE=1 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 --bind -s WASM=1 -O2 -s LEGACY_GL_EMULATION=0  -s GL_UNSAFE_OPTS=0 -s ASSERTIONS=1 -s GL_ASSERTIONS=1 -s INVOKE_RUN=0  -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s OFFSCREENCANVAS_SUPPORT=1")
endif()