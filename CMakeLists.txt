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

include(sdks/tfl/buildenv/cmake_util.cmake)
too_include(buildnr.cmake)
project(MinimalQtTestApp VERSION 0.1.${BUILD_NR}.0)
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetAppName ${PROJECT_NAME})

too_include(defaults.cmake)

set(TOO_USE_BOOST_ver1 "1" CACHE STRING "Use boost version ver1.ver2.ver3")
set(TOO_USE_BOOST_ver2 "64" CACHE STRING "Cf. TOO_USE_BOOST_ver1")
set(TOO_USE_BOOST_ver3 "0" CACHE STRING "Cf. TOO_USE_BOOST_ver1")

add_subdirectory(sdks)
add_subdirectory(gui/Qt/qt_qml_extensions) # for test app

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    too_include(qt.cmake)
    find_package(Qt5Quick) # Widgets contains a.o.t. Core and Gui
endif ()

file(GLOB_RECURSE TargetAppNameAsLibForTest_SRC_INCS "src/*.h*")

# Sources to be switched according to active GUI implementation.
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(TargetAppNameAsLibForTest_IMPL_SOURCES
        src/_gui/src_Qt/resource_string/resource_string.cpp
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

configure_file(${TargetAppName}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetAppName}_build_config.h)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${TargetAppNameAsLibForTest} PUBLIC src)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${tfl_INCLUDE_DIRS})
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${toolib_INCLUDE_DIRS})
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${uiwrap_INCLUDE_DIRS})
target_include_directories(${TargetAppNameAsLibForTest} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_include_directories(${TargetAppNameAsLibForTest} SYSTEM PUBLIC ${Qt5Quick_INCLUDE_DIRS})
endif ()

target_link_libraries(${TargetAppNameAsLibForTest} PUBLIC toolib)
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${TargetAppNameAsLibForTest} PUBLIC Qt5::Quick)
endif ()

too_set_target_defaults(${TargetAppNameAsLibForTest})

set(${TargetAppNameAsLibForTest}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${TargetAppNameAsLibForTest}: include directories" FORCE)


###############################################################################################

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    # Only purpose of this so far: prevent showing an additional console window in Qt apps (happens for msvc in way
    # impossible to overlook, but also for mingw builds when not starting from IDE).
    if (WIN32)
        set(WIN32_ON_OFF_SWITCH "WIN32")
    else ()
        set(WIN32_ON_OFF_SWITCH "")
    endif ()
    qt5_add_resources(TargetAppName_RESOURCES gui/Qt/qml.qrc)
    set(TargetAppName_IMPL_SOURCES
        src/_gui/src_Qt/main.cpp
    )
else ()
    set(TargetAppName_RESOURCES "")
    set(TargetAppName_IMPL_SOURCES
        src/_gui/src_WinCmdLine/main.cpp
    )
endif ()

too_include(uwp_tools.cmake)
too_obtain_UWP_resources(
    "MinimalQtTestApp"
    "Enterprise"
    "MinimalQtTestApp"
    ""
    ""
    ""
    "${PROJECT_VERSION}"
    "${CMAKE_SOURCE_DIR}/installer/_content_for_bindir"
    "#336699"
    ""
    TargetAppName_UWP_ASSETS
)

set(TargetAppName_input
    ${WIN32_ON_OFF_SWITCH}
    ${TargetAppName_UWP_ASSETS}
    ${TargetAppName_RESOURCES}
    ${TargetAppName_IMPL_SOURCES}
)

set(TargetAppName_QML_DIR ${CMAKE_SOURCE_DIR}/gui/Qt)

if (ANDROID)
    add_library(${TargetAppName} SHARED ${TargetAppName_input})
    include(${TOO_CMAKE_UTIL_DIR}/sdks/qt-android-cmake/AddQtAndroidApk.cmake)
    add_qt_android_apk(${TargetAppName}_apk ${TargetAppName}
        DEPENDS toolib
        VERSION_CODE 1
        NAME "MinimalQtTestApp"
        QML_SOURCE_DIR ${TargetAppName_QML_DIR}
        PACKAGE_NAME "org.enterprise.minimalqttestapp"
        BUILDTOOLS_REVISION "26.0.1"
    )
else ()
    add_executable(${TargetAppName} ${TargetAppName_input})
endif ()

target_link_libraries(${TargetAppName} PRIVATE ${TargetAppNameAsLibForTest})

too_set_target_defaults(${TargetAppName})

too_include(qt_deploy.cmake)
too_qt_deploy(${TargetAppName} ${TargetAppName_QML_DIR})


################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

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

target_link_libraries(${TargetAppNameTest} PRIVATE gtest_main ${TargetAppNameAsLibForTest})

too_set_target_defaults(${TargetAppNameTest})

# commented-out since until now the test app isn't a Qt app
#too_qt_deploy(${TargetAppNameTest} ${TargetAppName_QML_DIR})

too_run_target_postbuild(${TargetAppNameTest})
