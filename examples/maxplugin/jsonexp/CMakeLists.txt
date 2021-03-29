set_property(GLOBAL PROPERTY USE_FOLDERS ON)
file(GLOB_RECURSE SRC_FILES *.h *.cpp *.rc *.def) 
IF(MSVC)
    add_definitions(-DNOMINMAX -DUNICODE -D_UNICODE)
ENDIF()

find_package(3DSMAX)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(rapidjson CONFIG REQUIRED)

set(target_name jsonexp)
set(target_ext .dle)
set(install_dir "C:\\Program Files\\Autodesk\\3ds Max 2014\\stdplugs")

add_library(${target_name} SHARED ${SRC_FILES})
set_target_properties(${target_name} PROPERTIES SUFFIX ${target_ext})

target_include_directories(${target_name} PRIVATE ${3DSMAX_INCLUDE_DIR})
target_link_directories(${target_name} PRIVATE ${3DSMAX_LIB_DIR})
target_link_libraries(${target_name} PRIVATE ${3DSMAX_LIBRARIES} nlohmann_json)
# target_precompile_headers(${target_name} PRIVATE stdafx.h)
# IGNORE_PRECOMPILED_HEADER(IGNORE_PCH_SRC_LIST)

install(TARGETS ${target_name} RUNTIME DESTINATION ${install_dir} )

