
project(qt5Example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB APP_HEADER *.h) 
file(GLOB APP_SOURCE *.cpp *.qrc *.ui) 
#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

add_executable(${PROEJCT_NAME} ${APP_SOURCE})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)