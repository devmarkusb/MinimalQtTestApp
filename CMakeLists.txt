# Some notes for the occasional CMake user.
#
# 1 Debug or Release or...
# start cmake with arg either
# -DCMAKE_BUILD_TYPE=Debug
# or
# -DCMAKE_BUILD_TYPE=Release
# Other options are: MinSizeRel, RelWithDebInfo
# *Important notice* CMAKE_BUILD_TYPE cannot properly be used to query the current build type,
# at least not in a naive way, at least not for multiple build config. environments such as msvc.
#
# 2 Dynamic vs Static C/C++ runtime lib
# There is no support for static runtime libs! You could achieve this
# for MSVC, but this is not recommended. You or other will end up in
# conflicts soon.

cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(sdks/ToolsFL/tooBuildEnv)
include(${TOO_CMAKE_INC_BUILDNR})
project(MinimalQtTestApp VERSION 0.1.${BUILD_NR}.0)
message(STATUS "project version: " ${PROJECT_VERSION})
option(DEPLOYMENT_BUILD "do a final release build for deployment" $ENV{DeployArtifacts})
set(TargetAppName ${PROJECT_NAME})

include(${TOO_CMAKE_INC_DEFAULTS})

add_subdirectory(sdks)

file(GLOB_RECURSE TargetAppNameAsLibForTest_SRC_INCS "src/*.h*")

# Sources to be switched according to active GUI implementation.
set(TargetAppNameAsLibForTest_IMPL_SOURCES
    src/_gui/src_WinCmdLine/resource_string.cpp
)

set(TargetAppNameAsLibForTest_SOURCES
    src/_gui/interface/resource_string.cpp

   ${TargetAppNameAsLibForTest_IMPL_SOURCES}
)

set(TargetAppNameAsLibForTest ${PROJECT_NAME}AsLibForTest)

add_library(${TargetAppNameAsLibForTest} STATIC
    ${TargetAppNameAsLibForTest_SOURCES}
    ${TargetAppNameAsLibForTest_SRC_INCS}
)

set_target_properties(${TargetAppNameAsLibForTest} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (WIN32)
    set_property(TARGET ${TargetAppNameAsLibForTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppNameAsLibForTest} PROPERTIES DEBUG_POSTFIX "d")

configure_file(${TargetAppName}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetAppName}_build_config.h)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${TargetAppNameAsLibForTest} PUBLIC src)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${tooBasicCodeSupport_INCLUDE_DIRS})
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${tooString_INCLUDE_DIRS})

target_link_libraries(${TargetAppNameAsLibForTest}) # don't link header-only libs

set(${TargetAppNameAsLibForTest}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${TargetAppNameAsLibForTest}: include directories" FORCE)


###############################################################################################

add_executable(${TargetAppName}
    src/_gui/src_WinCmdLine/main.cpp
)

if (WIN32)
    set_property(TARGET ${TargetAppName} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppName} PROPERTIES DEBUG_POSTFIX "d")

target_link_libraries(${TargetAppName} PRIVATE ${TargetAppNameAsLibForTest})


################################################################################################

set(TargetAppNameTest ${PROJECT_NAME}Test)

set(TargetAppNameTest_IMPL_SOURCES
    src/_gui/src_WinCmdLine/resource_string.test.cpp
)

set(TargetAppNameTest_SOURCES
    src/_gui/interface/resource_ids.test.cpp
    src/_gui/interface/resource_string.test.cpp

    ${TargetAppNameTest_IMPL_SOURCES}
)

add_executable(${TargetAppNameTest}
	${TargetAppNameTest_SOURCES}
    ${TargetAppNameAsLibForTest_SRC_INCS}
)

if (WIN32)
    set_property(TARGET ${TargetAppNameTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppNameTest} PROPERTIES DEBUG_POSTFIX "d")

target_link_libraries(${TargetAppNameTest} PRIVATE gtest_main ${TargetAppName})


add_custom_command(
  TARGET ${TargetAppNameTest} POST_BUILD
  COMMAND ${TargetAppNameTest}
)
