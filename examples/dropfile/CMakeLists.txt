
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
file(GLOB_RECURSE SRC_FILES *.h *.cpp *.rc *.def) 

IF(MSVC)
    add_definitions(-DNOMINMAX -DUNICODE -D_UNICODE)
ENDIF()

set(current_target "dropfile")

add_executable(${current_target} ${SRC_FILES})
