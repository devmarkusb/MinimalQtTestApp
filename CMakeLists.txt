cmake_minimum_required(VERSION 3.4.1)

include(sdks/util/buildenv/cmake_util.cmake)
ul_include(buildnr.cmake)
project(MinimalQtTestApp VERSION 0.1.${BUILD_NR}.0)
message(STATUS "${PROJECT_NAME} version: " ${PROJECT_VERSION})
set(target ${PROJECT_NAME})

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
    find_package(Qt5 COMPONENTS Quick QuickControls2 Svg)
endif ()

file(GLOB_RECURSE targetAsLibForTest_SRC_INCS "src/*.h*")

# Sources to be switched according to active GUI implementation.
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    set(targetAsLibForTest_IMPL_SOURCES
        src/_gui/src_Qt/resource_string/resource_string.cpp
    )
elseif (uiwrap_USE_IMPLEMENTATION STREQUAL "wx")
else ()
    set(targetAsLibForTest_IMPL_SOURCES
        src/_gui/src_WinCmdLine/resource_string.cpp
    )
endif ()

set(targetAsLibForTest_SOURCES
    src/_gui/interface/resource_string.cpp

   ${targetAsLibForTest_IMPL_SOURCES}
)

set(targetAsLibForTest ${PROJECT_NAME}AsLibForTest)

add_library(${targetAsLibForTest} STATIC
    ${targetAsLibForTest_SOURCES}
    ${targetAsLibForTest_SRC_INCS}
)

configure_file(${target}_build_config.h.in ${CMAKE_BINARY_DIR}/${target}_build_config.h)
target_include_directories(${targetAsLibForTest} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${targetAsLibForTest} PUBLIC src)
target_include_directories(${targetAsLibForTest} PUBLIC ${tfl_INCLUDE_DIRS})
target_include_directories(${targetAsLibForTest} PUBLIC ${uiwrap_INCLUDE_DIRS})
if (uiwrap_USE_IMPLEMENTATION STREQUAL "own")
    target_include_directories(${targetAsLibForTest} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
endif()

target_link_libraries(${targetAsLibForTest} PUBLIC uiwrap)
if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    target_link_libraries(${targetAsLibForTest} PUBLIC Qt::Quick Qt::QuickControls2 Qt::Svg)
endif ()

ul_set_target_defaults(${targetAsLibForTest})

set(${targetAsLibForTest}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${targetAsLibForTest}: include directories" FORCE)


###############################################################################################

if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    # Only purpose of this so far: prevent showing an additional console window in Qt apps (happens for msvc in way
    # impossible to overlook, but also for mingw builds when not starting from IDE).
    if (WIN32)
        set(WIN32_ON_OFF_SWITCH "WIN32")
    else ()
        set(WIN32_ON_OFF_SWITCH "")
    endif ()
    qt5_add_resources(target_RESOURCES gui/Qt/qml.qrc)
    set(target_IMPL_SOURCES
        src/_gui/src_Qt/main.cpp
    )
else ()
    set(target_RESOURCES "")
    set(target_IMPL_SOURCES
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
    target_UWP_ASSETS
)

set(target_input
    ${WIN32_ON_OFF_SWITCH}
    ${target_UWP_ASSETS}
    ${target_RESOURCES}
    ${target_IMPL_SOURCES}
)

set(target_QML_DIR ${CMAKE_SOURCE_DIR}/gui/Qt)

if (ANDROID)
    add_library(${target} SHARED ${target_input})
else ()
    add_executable(${target} ${target_input})
endif ()

target_link_libraries(${target} PRIVATE ${targetAsLibForTest})

ul_set_target_defaults(${target})

set_property(SOURCE qrc_qml.cpp PROPERTY SKIP_AUTOMOC ON)

ul_include(qt_deploy.cmake)
ul_qt_deploy(${target} ${target_QML_DIR})

if (ANDROID)
    include(${UL_CMAKE_UTIL_DIR}/qt-android-cmake.cmake)
    string(TOLOWER ${target} targetLowerCase)
    add_qt_android_apk(${target}_apk ${target}
            DEPENDS uiwrap
            VERSION_CODE 1
            # uncomment the following two lines for a signed release build apk
            # *and* increment VERSION_CODE above
            KEYSTORE ${UL_ANDROID_SIGN_KEY_PATH} key
            KEYSTORE_PASSWORD ${UL_ANDROID_SIGN_KEY_PASS}
            NAME ${target}
            PACKAGE_SOURCES ${UL_CMAKE_UTIL_DIR}/android
            PACKAGE_NAME "org.enterprise.${targetLowerCase}"
            BUILDTOOLS_REVISION "28.0.3")
endif ()


################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)


if (uiwrap_USE_IMPLEMENTATION STREQUAL "qt")
    file(GLOB_RECURSE targetTest_IMPL_SOURCES
        "src/_gui/src_Qt/*.test.cpp"
        )
else ()
    file(GLOB_RECURSE targetTest_IMPL_SOURCES
        "src/_gui/src_WinCmdLine/*.test.cpp"
        )
endif ()

set(targetTest_SOURCES
    src/_gui/interface/resource_ids.test.cpp
    src/_gui/interface/resource_string.test.cpp

    ${targetTest_IMPL_SOURCES}
)

add_executable(${targetTest}
	${targetTest_SOURCES}
    ${targetAsLibForTest_SRC_INCS}
)

target_link_libraries(${targetTest} PRIVATE gtest_main ${targetAsLibForTest})

ul_set_target_defaults(${targetTest})

# commented-out since until now the test app isn't a Qt app
#ul_qt_deploy(${targetTest} ${target_QML_DIR})

ul_run_target_postbuild(${targetTest})
