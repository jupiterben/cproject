
set(CMAKE_CXX_STANDARD 14)

## VCPKG
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
if(MSVC)
  set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "VCPKG Target Triplet to use")
else()
  set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "VCPKG Target Triplet to use")
endif()


## Platform Configuration
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR};${CMAKE_MODULE_PATH}")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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