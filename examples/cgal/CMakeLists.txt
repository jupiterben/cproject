#find 3rd party
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

if(CGAL_Qt5_FOUND)
  #required to use basic_viewer
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()
if ( CGAL_FOUND )
  #create the executable of the application
  create_single_source_cgal_program("cgalqt5.cpp")
  if(CGAL_Qt5_FOUND )
    #link it with the required CGAL libraries
    target_link_libraries(cgalqt5 PUBLIC CGAL::CGAL_Qt5)
  endif()
else()
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
endif()