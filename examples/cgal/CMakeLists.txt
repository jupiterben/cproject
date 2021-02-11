# #find 3rd party
# find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

# if(CGAL_Qt5_FOUND)
#   #required to use basic_viewer
#   add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
# endif()
# if ( CGAL_FOUND )
#   #create the executable of the application
#   create_single_source_cgal_program("cgalqt5.cpp")
#   if(CGAL_Qt5_FOUND )
#     #link it with the required CGAL libraries
#     target_link_libraries(cgalqt5 PUBLIC CGAL::CGAL_Qt5)
#   endif()
# else()
#     message(STATUS "This program requires the CGAL library, and will not be compiled.")
# endif()


file(GLOB APP_HEADER *.h) 
file(GLOB APP_SOURCE *.cpp) 
#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

find_package(CGAL REQUIRED COMPONENTS Core)

project(cgalExample)

add_executable(${PROJECT_NAME} ${APP_SOURCE})
target_link_libraries(${PROJECT_NAME} CGAL::CGAL)

#编译选项
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W3)
endif()