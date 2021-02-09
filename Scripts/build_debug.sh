
#!/usr/bin/bash
#获取脚本所在目录 
#不管是软连接还是硬链接都可以获取
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ];
do 
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
CURRENT_DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

#转到Tool/emsdk目录
cd $CURRENT_DIR

#创建编译目录
cd ..
mkdir build
cd build
mkdir debug
cd debug

# 使用cmake编译
#cmake ../.. "-DCMAKE_TOOLCHAIN_FILE=/root/vcpkg/scripts/buildsystems/vcpkg.cmake"  "-DCMAKE_BUILD_TYPE=Debug"
# CMAKE_TOOLCHAIN_FILE=./emsdk/fastcomp/emscripten/cmake/Modules/Platform/Emscripten.cmake
# DEMSCRIPTEN_ROOT_PATH=./emsdk/fastcomp/emscripten
VERBOSE=1
cmake ../.. "-DCMAKE_BUILD_TYPE=Debug"
cmake --build .

#run result
# cd ../..
# node CMakeBuild/Debug/UnitTest/UnitTest.js 