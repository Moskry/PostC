cmake_minimum_required(VERSION 3.16)
project(PostC)

if(NOT CMAKE_BUILD_TYPE)
    set(default_build_type "Debug")
    message(STATUS "Set the build type to `${default_build_type}` as none was specified.")
    set(CMAKE_BUILD_TYPE ${default_build_type} CACHE STRING "Chooce the build type." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "myProject Build Type: ${CMAKE_BUILD_TYPE}")

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 64)
    set(OSBitness 64)
endif()

set(FullOutputDir "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullOutputDir}/static_libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullOutputDir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")

include(CTest)
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE FNS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/fns/*.c")
file(GLOB_RECURSE HDRS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/hdr/*.h")
file(GLOB_RECURSE test_paths CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c")
file(GLOB_RECURSE example_tests CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.c")

# Set the version for myProject
set(myProject_Version_Major 0)
set(myProject_Version_Minor 1)
set(myProject_Version_Patch 0)
set(myProject_Version_Status "-dev")
set(PROJECT_VERSION
    "${myProject_Version_Major}.${myProject_Version_Minor}.${myProject_Version_Patch}${myProject_Version_Status}"
)
message(STATUS "${PROJECT_NAME} version: ${PROJECT_VERSION}")

add_library("${PROJECT_NAME}_lib" ${FNS} ${HDRS})
target_include_directories("${PROJECT_NAME}_lib" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/hdr")

# Tests (CTest)
foreach(source IN LISTS test_paths)
    file(RELATIVE_PATH relative_path "${CMAKE_CURRENT_SOURCE_DIR}/test" "${source}")

    string(REPLACE "/" "_" tests "${relative_path}")
    string(REPLACE "\\" "_" tests "${tests}")
    string(REGEX REPLACE "\\.c$" "" tests "${tests}")

    add_executable("${tests}" "${source}")
    set_target_properties("${tests}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test-bin")
    target_link_libraries("${tests}" PRIVATE "${PROJECT_NAME}_lib")
    add_test(NAME "${tests}" COMMAND "$<TARGET_FILE:${tests}>")
endforeach()

# Examples
foreach(source IN LISTS example_tests)
    file(RELATIVE_PATH relative_path "${CMAKE_CURRENT_SOURCE_DIR}/examples" "${source}")

    string(REPLACE "/" "_" example "${relative_path}")
    string(REPLACE "\\" "_" example "${example}")
    string(REGEX REPLACE "\\.c$" "" example "${example}")

    add_executable(${example} ${source})
    set_target_properties("${example}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples/bin")
    target_link_libraries("${example}" PRIVATE "${PROJECT_NAME}_lib")

endforeach()

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/hdr")
