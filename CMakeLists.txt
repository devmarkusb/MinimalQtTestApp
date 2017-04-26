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
include(${TOO_CMAKE_INC_DEPLOYMENT_BUILD})
set(TargetAppName ${PROJECT_NAME})

include(${TOO_CMAKE_INC_DEFAULTS})

add_subdirectory(sdks)

include (${TOO_CMAKE_INC_QT})
find_package(Qt5Quick) # Widgets contains a.o.t. Core and Gui

file(GLOB_RECURSE TargetAppNameAsLibForTest_SRC_INCS "src/*.h*")

# Sources to be switched according to active GUI implementation.
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(TargetAppNameAsLibForTest_IMPL_SOURCES
        src/_gui/src_Qt/resource_string.cpp
        )
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
else ()
    set(TargetAppNameAsLibForTest_IMPL_SOURCES
        src/_gui/src_WinCmdLine/resource_string.cpp
        )
endif ()

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
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_include_directories(${TargetAppNameAsLibForTest} SYSTEM PUBLIC ${Qt5Quick_INCLUDE_DIRS})
endif ()

target_link_libraries(${TargetAppNameAsLibForTest}) # don't link header-only libs
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${TargetAppNameAsLibForTest} PUBLIC Qt5::Quick)
endif ()

set(${TargetAppNameAsLibForTest}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${TargetAppNameAsLibForTest}: include directories" FORCE)


###############################################################################################

# Only purpose of this so far: prevent showing an additional console window in Qt apps (happens for msvc in way
# impossible to overlook, but also for mingw builds when not starting from IDE).
if (WIN32)
    set(WIN32_ON_OFF_SWITCH "WIN32")
else ()
    set(WIN32_ON_OFF_SWITCH "")
endif ()

qt5_add_resources(TargetAppName_RESOURCES gui/Qt/qml.qrc)

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(TargetAppName_IMPL_SOURCES
        src/_gui/src_Qt/main.cpp
    )
elseif ()
    set(TargetAppName_IMPL_SOURCES
        src/_gui/src_WinCmdLine/main.cpp
    )
endif ()

add_executable(${TargetAppName}
    ${WIN32_ON_OFF_SWITCH}
    ${TargetAppName_RESOURCES}
    ${TargetAppName_IMPL_SOURCES}
)

if (WIN32)
    set_property(TARGET ${TargetAppName} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppName} PROPERTIES DEBUG_POSTFIX "d")

target_link_libraries(${TargetAppName} PRIVATE ${TargetAppNameAsLibForTest})


################################################################################################

set(TargetAppNameTest ${PROJECT_NAME}Test)


if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    file(GLOB_RECURSE TargetAppNameTest_IMPL_SOURCES
        "src/_gui/src_Qt/*.test.cpp"
        )
else ()
    file(GLOB_RECURSE TargetAppNameTest_IMPL_SOURCES
        "src/_gui/src_WinCmdLine/*.test.cpp"
        )
endif ()

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

target_link_libraries(${TargetAppNameTest} PRIVATE gtest_main ${TargetAppNameAsLibForTest})

# Postbuild step before running the test or the program. Here we use the program exe to determine the needed
# binaries for both the program and the test exe - that was sufficient so far. The action needs to be coupled
# to the postbuild of the test - and not the app - because that's what is getting called immediately automatically.
# But it also always *must* be done, securing a properly complete deployment build for the app exe.
if (WIN32)
    if (NOT MSVC)
        if (CMAKE_BUILD_TYPE MATCHES Debug)
            set(TargetBuildType_for_qtdeploy "--debug")
        else ()
            set(TargetBuildType_for_qtdeploy "--release")
        endif ()
    else ()
        set(TargetBuildType_for_qtdeploy)
    endif ()
    add_custom_command(
        TARGET ${TargetAppNameTest} POST_BUILD
        COMMAND qtdeploy_autodepends_win.bat "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" "${Qt_ver_comp_path}\\bin"
            ${TargetBuildType_for_qtdeploy} --qmldir ..\\gui\\Qt $<TARGET_FILE_NAME:${TargetAppName}>
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
endif ()

add_custom_command(
  TARGET ${TargetAppNameTest} POST_BUILD
  COMMAND ${TargetAppNameTest}
)
