file(GLOB APP_HEADER *.h)
file(GLOB APP_SOURCE *.cpp)

# 在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

IF(MSVC)
    add_definitions(-DNOMINMAX -DUNICODE -D_UNICODE)
ENDIF()

project(usnExample)

add_executable(${PROJECT_NAME} ${APP_SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC .)

# 编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
endif()