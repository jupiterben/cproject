file(GLOB_RECURSE APP_HEADER *.h) 
file(GLOB_RECURSE APP_SOURCE *.cpp *.rc *.def) 

#在IDE工程中将文件进行分组(e.g. Visual Studio)
source_group("Header" FILES ${APP_HEADER})
source_group("Source" FILES ${APP_SOURCE})

set(target_name xml)
add_executable(${target_name} ${APP_SOURCE})

# google benchmark
find_package(benchmark CONFIG REQUIRED)
target_link_libraries(${target_name} PRIVATE benchmark::benchmark benchmark::benchmark_main)

# tinyxml 
find_package(tinyxml CONFIG REQUIRED)
target_link_libraries(${target_name} PRIVATE unofficial-tinyxml::unofficial-tinyxml)

# tinyxml2
find_package(tinyxml2 CONFIG REQUIRED)
target_link_libraries(${target_name} PRIVATE tinyxml2::tinyxml2)

# rapid xml
find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")
target_include_directories(${target_name} PRIVATE ${RAPIDXML_INCLUDE_DIRS})

# expat 
find_package(expat CONFIG REQUIRED)
target_link_libraries(${target_name} PRIVATE expat::expat)

# libxml2
find_package(LibXml2 REQUIRED)
target_include_directories(${target_name} PRIVATE ${LIBXML2_INCLUDE_DIR})
target_link_libraries(${target_name} PRIVATE ${LIBXML2_LIBRARIES})

# pugixml
find_package(pugixml CONFIG REQUIRED)
target_link_libraries(${target_name} PRIVATE pugixml pugixml::shared pugixml::pugixml)