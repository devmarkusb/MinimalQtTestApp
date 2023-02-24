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

include(sdks/util/buildenv/cmake_util.cmake)
ul_include(buildnr.cmake)
project(MinimalQtTestApp VERSION 0.1.${BUILD_NR}.0)
message(STATUS "${PROJECT_NAME} version: " ${PROJECT_VERSION})
set(Target ${PROJECT_NAME})

ul_include(defaults.cmake)

ul_include(clang-format.cmake)
ul_include(githooks.cmake)

if (uiwrap_USE_IMPLEMENTATION STREQUAL "own")
    ul_include(boost.cmake)
    ul_find_boost(${UL_USE_BOOST_ver1} ${UL_USE_BOOST_ver2} ${UL_USE_BOOST_ver3} "")
endif()

add_subdirectory(sdks)
add_subdirectory(gui/Qt/qt_qml_extensions) # for test app

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    ul_include(qt5.cmake)
    find_package(Qt5 COMPONENTS Quick) # Widgets contains a.o.t. Core and Gui
endif ()

file(GLOB_RECURSE TargetAsLibForTest_SRC_INCS "src/*.h*")

# Sources to be switched according to active GUI implementation.
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(TargetAsLibForTest_IMPL_SOURCES
        src/_gui/src_Qt/resource_string/resource_string.cpp
    )
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
else ()
    set(TargetAsLibForTest_IMPL_SOURCES
        src/_gui/src_WinCmdLine/resource_string.cpp
    )
endif ()

set(TargetAsLibForTest_SOURCES
    src/_gui/interface/resource_string.cpp

   ${TargetAsLibForTest_IMPL_SOURCES}
)

set(TargetAsLibForTest ${PROJECT_NAME}AsLibForTest)

add_library(${TargetAsLibForTest} STATIC
    ${TargetAsLibForTest_SOURCES}
    ${TargetAsLibForTest_SRC_INCS}
)

configure_file(${Target}_build_config.h.in ${CMAKE_BINARY_DIR}/${Target}_build_config.h)
target_include_directories(${TargetAsLibForTest} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${TargetAsLibForTest} PUBLIC src)
target_include_directories(${TargetAsLibForTest} PUBLIC ${tfl_INCLUDE_DIRS})
target_include_directories(${TargetAsLibForTest} PUBLIC ${uiwrap_INCLUDE_DIRS})
if (uiwrap_USE_IMPLEMENTATION STREQUAL "own")
    target_include_directories(${TargetAsLibForTest} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
endif()
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_include_directories(${TargetAsLibForTest} SYSTEM PUBLIC ${Qt5Quick_INCLUDE_DIRS})
endif ()

target_link_libraries(${TargetAsLibForTest} PUBLIC uiwrap)
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${TargetAsLibForTest} PUBLIC Qt5::Quick)
endif ()

ul_set_target_defaults(${TargetAsLibForTest})

set(${TargetAsLibForTest}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${TargetAsLibForTest}: include directories" FORCE)


###############################################################################################

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    # Only purpose of this so far: prevent showing an additional console window in Qt apps (happens for msvc in way
    # impossible to overlook, but also for mingw builds when not starting from IDE).
    if (WIN32)
        set(WIN32_ON_OFF_SWITCH "WIN32")
    else ()
        set(WIN32_ON_OFF_SWITCH "")
    endif ()
    qt5_add_resources(Target_RESOURCES gui/Qt/qml.qrc)
    set(Target_IMPL_SOURCES
        src/_gui/src_Qt/main.cpp
    )
else ()
    set(Target_RESOURCES "")
    set(Target_IMPL_SOURCES
        src/_gui/src_WinCmdLine/main.cpp
    )
endif ()

ul_include(uwp_tools.cmake)
ul_obtain_UWP_resources(
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
    Target_UWP_ASSETS
)

set(Target_input
    ${WIN32_ON_OFF_SWITCH}
    ${Target_UWP_ASSETS}
    ${Target_RESOURCES}
    ${Target_IMPL_SOURCES}
)

set(Target_QML_DIR ${CMAKE_SOURCE_DIR}/gui/Qt)

if (ANDROID)
    add_library(${Target} SHARED ${Target_input})
else ()
    add_executable(${Target} ${Target_input})
endif ()

target_link_libraries(${Target} PRIVATE ${TargetAsLibForTest})

ul_set_target_defaults(${Target})

set_property(SOURCE qrc_qml.cpp PROPERTY SKIP_AUTOMOC ON)

ul_include(qt_deploy.cmake)
ul_qt_deploy(${Target} ${Target_QML_DIR})

if (ANDROID)
    include(${UL_CMAKE_UTIL_DIR}/qt-android-cmake.cmake)
    add_qt_android_apk(${Target}_apk ${Target}
            DEPENDS uiwrap
            VERSION_CODE 1
            # uncomment the following two lines for a signed release build apk
            # *and* increment VERSION_CODE above
            KEYSTORE ${CMAKE_SOURCE_DIR}/doc/stores/googleplaystore/upload_keystore.jks upload
            KEYSTORE_PASSWORD "rMi5Y53Qxx5Z5jWH"
            NAME "MinimalQtTestApp"
            QML_SOURCE_DIR ${Target_QML_DIR}
            PACKAGE_NAME "org.enterprise.minimalqttestapp"
            BUILDTOOLS_REVISION "33.0.2")
endif ()


################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(TargetTest ${PROJECT_NAME}Test)


if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    file(GLOB_RECURSE TargetTest_IMPL_SOURCES
        "src/_gui/src_Qt/*.test.cpp"
        )
else ()
    file(GLOB_RECURSE TargetTest_IMPL_SOURCES
        "src/_gui/src_WinCmdLine/*.test.cpp"
        )
endif ()

set(TargetTest_SOURCES
    src/_gui/interface/resource_ids.test.cpp
    src/_gui/interface/resource_string.test.cpp

    ${TargetTest_IMPL_SOURCES}
)

add_executable(${TargetTest}
	${TargetTest_SOURCES}
    ${TargetAsLibForTest_SRC_INCS}
)

target_link_libraries(${TargetTest} PRIVATE gtest_main ${TargetAsLibForTest})

ul_set_target_defaults(${TargetTest})

# commented-out since until now the test app isn't a Qt app
#ul_qt_deploy(${TargetTest} ${Target_QML_DIR})

ul_run_target_postbuild(${TargetTest})
