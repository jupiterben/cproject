cmake_minimum_required(VERSION 3.13)

#
project(CProject)

#
set(CMAKE_CXX_STANDARD 14)

# 添加CMake模块搜索路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# 设置编译选项
if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG   "-Od" )             # 调试包不优化
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )   # release包优化
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZI")
elseif(EMSCRIPTEN)
  set(CMAKE_CXX_FLAGS_DEBUG "-g4")
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s SINGLE_FILE=1 -s WASM=1" CACHE STRING "")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
endif()

# 不生成ZERO_CHECK工程 这个工程用于修改cmake文件后重新运行cmake命令
set(CMAKE_SUPPRESS_REGENERATION true)  

# VS下忽略编译报错 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
if(MSVC)
    # disable warnings
    set(MSVC_DISABLED_WARNINGS_LIST
      "C4018" # 'expression' : signed/unsigned mismatch
    )
    string(REPLACE "C" " -wd" MSVC_DISABLED_WARNINGS_STR ${MSVC_DISABLED_WARNINGS_LIST})
    string(REGEX REPLACE "[/-]W[1234][ ]?" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -W4 ${MSVC_DISABLED_WARNINGS_STR}")
endif()

# add_subdirectory(main)
add_subdirectory(examples)

