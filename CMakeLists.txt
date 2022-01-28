cmake_minimum_required(VERSION 3.6.0)
project(wordle_clone)

option(BUILD_DEBUG "Build a debug release" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directory.
include_directories("include")

#Adding sources...
set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

add_library(utf8-tools STATIC IMPORTED)
add_library(terminal-tools STATIC IMPORTED)
add_library(tools STATIC IMPORTED)
set_target_properties(utf8-tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libutf8tools.a)
set_target_properties(terminal-tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libterminal-tools.a)
set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.a)

add_executable(wordle_clone src/main.cpp ${SOURCE})
target_link_libraries(wordle_clone utf8-tools terminal-tools tools)
