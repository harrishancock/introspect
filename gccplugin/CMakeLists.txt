cmake_minimum_required(VERSION 2.8.11)

project(reflection CXX)

find_program(DEF_TARGET_GCC gcc)
set(TARGET_GCC ${DEF_TARGET_GCC} CACHE FILEPATH "gcc that will actually use the plugin")

execute_process(COMMAND ${TARGET_GCC} -print-file-name=plugin
  OUTPUT_VARIABLE GCC_PLUGIN_DIR
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "${PROJECT_NAME}: Found gcc-plugin: ${GCC_PLUGIN_DIR}")

set(SOURCES reflection.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
#target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)
target_include_directories(${PROJECT_NAME} PRIVATE ${GCC_PLUGIN_DIR}/include)

enable_testing()
add_test(NAME CompileFoo
  COMMAND ${TARGET_GCC} -std=c++11 -fplugin=$<TARGET_FILE:${PROJECT_NAME}>
                        -o foo.o ${PROJECT_SOURCE_DIR}/tests/foo.cpp -c)
