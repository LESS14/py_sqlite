cmake_minimum_required(VERSION 3.10)
project(py_sqlite)

find_package(Python COMPONENTS Development)

include_directories("lib/python/")
include_directories("lib/sqlite/")
add_library(py_sqlite MODULE main.cpp lib/sqlite/sqlite3.c)
target_link_libraries(py_sqlite PRIVATE ${Python_LIBRARIES})
set_target_properties(py_sqlite PROPERTIES PREFIX "")
set_target_properties(py_sqlite PROPERTIES SUFFIX ".pyd")

set_target_properties(py_sqlite PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
