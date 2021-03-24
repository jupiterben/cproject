cmake_minimum_required(VERSION 3.13)

project(CProject)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG   "-Od" )             # 调试包不优化
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )   # release包优化
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZI")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
elseif(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS_DEBUG "-g4")
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
  #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s SINGLE_FILE=1 -s WASM=1" CACHE STRING "")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
endif()


set(CMAKE_SUPPRESS_REGENERATION true)  # 不生成ZERO_CHECK工程 这个工程用于修改cmake文件后重新运行cmake命令


#add_subdirectory(main)
add_subdirectory(examples)
#add_subdirectory(test)





