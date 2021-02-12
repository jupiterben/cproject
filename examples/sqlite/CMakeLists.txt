cmake_minimum_required(VERSION 3.0)
project(sqliteSample)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(CGAL CONFIG REQUIRED)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(${PROJECT_NAME} PRIVATE CGAL::CGAL)
