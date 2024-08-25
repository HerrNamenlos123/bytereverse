cmake_minimum_required(VERSION 3.16)
project(ByteReverser VERSION 0.4.0)

include("cmake/dependencies.cmake")
add_subdirectory("reverse")
add_subdirectory("cli")

configure_file(cmake/version.h.in version.h)
target_include_directories(reverse PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

b_add_executable(${PROJECT_NAME})

file(GLOB_RECURSE HEADER_FILES "include/*")
file(GLOB_RECURSE RESOURCE_FILES "resource/*")
b_add_source_files(${PROJECT_NAME} ${HEADER_FILES} ${RESOURCE_FILES} "cmake/version.h.in"
    src/BatteryApp.cpp
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
# target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:$<$<CONFIG:Debug>:CONSOLE>$<$<NOT:$<CONFIG:Debug>>:WINDOWS>")

target_link_libraries(${PROJECT_NAME} battery::core)
target_link_libraries(${PROJECT_NAME} banana)
target_link_libraries(${PROJECT_NAME} banana-default)
target_link_libraries(${PROJECT_NAME} banana-winapi)
