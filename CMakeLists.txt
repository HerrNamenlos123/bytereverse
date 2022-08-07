cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME ByteReverser)
project(${TARGET_NAME} VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})





################
# Dependencies #
################

if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/Battery/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/cereal/CMakeLists.txt")
set(GET_SUB true)
endif ()

if (GET_SUB)
find_package(Git)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update in ${CMAKE_CURRENT_LIST_DIR}")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
endif()

if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/Battery/CMakeLists.txt"
    OR NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/modules/cereal/CMakeLists.txt")
message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif ()

add_subdirectory("modules/Battery")

set(SKIP_PERFORMANCE_COMPARISON true)
add_subdirectory("modules/cereal")



#########################
# Executable definition #
#########################

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "include/*")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
file(GLOB_RECURSE RES_FILES CONFIGURE_DEPENDS "resource/*")

add_executable(${TARGET_NAME} ${SRC_FILES} ${HEADER_FILES} ${RES_FILES})

target_include_directories(${TARGET_NAME} PUBLIC include)
target_include_directories(${TARGET_NAME} PUBLIC resource)
target_include_directories(${TARGET_NAME} PUBLIC build)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_14)
set_target_properties(${TARGET_NAME} PROPERTIES CXX_EXTENSIONS OFF)

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE "/MT$<$<CONFIG:Debug>:d>")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wno-psabi)
endif()




############################
# Preprocessor definitions #
############################

if (WIN32)
    target_compile_definitions(${TARGET_NAME} PRIVATE
       WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
       NOMINMAX                 # Prevents Windows.h from defining min/max as macros 
       _CRT_SECURE_NO_WARNINGS
    )
endif()




###########
# Version #
###########

configure_file(version.h.in version.h)





###########
# Linking #
###########

target_link_libraries(${TARGET_NAME} Battery::Battery)
target_link_libraries(${TARGET_NAME} cereal::cereal)




#######
# IDE #
#######

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/resource" PREFIX "Resource Files" FILES ${RES_FILES})


###########
# Install #
###########

install(
    TARGETS ${TARGET_NAME}
    EXPORT ${TARGET_NAME}Targets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)
