cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_FLAGS "-Wall")
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "linux/")
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
project("GraphColoring")

find_package (PkgConfig REQUIRED)
pkg_check_modules(PACKAGES REQUIRED gtk+-3.0 freetype2 cairo-ft)

file(GLOB_RECURSE PROJECT_SRC src/*.cpp)
add_executable(GraphColoring ${PROJECT_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE src ${PACKAGES_INCLUDE_DIRS})
link_directories(${PACKAGES_LIBRARY_DIRS})
add_definitions(${PACKAGES_CFLAGS_OTHER})
target_link_libraries(${PROJECT_NAME} m ${PACKAGES_LIBRARIES})
add_custom_command(TARGET GraphColoring POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_directory
		assets $<TARGET_FILE_DIR:GraphColoring>/assets)