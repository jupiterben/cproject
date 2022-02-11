file(GLOB_RECURSE SRC_FILES *.h *.cxx *.cpp) 

project(eigenSample)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# 寻找Eigen库
find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)