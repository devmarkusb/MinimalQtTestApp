cmake_minimum_required(VERSION 3.16)

include(sdks/util/buildenv/cmake_util.cmake)
ul_include(buildnr.cmake)
project(MinimalQtTestApp VERSION 0.1.${BUILD_NR}.0 LANGUAGES CXX)
message(STATUS "${PROJECT_NAME} version: " ${PROJECT_VERSION})
set(target ${PROJECT_NAME})

ul_include(defaults.cmake)

ul_include(clang-format.cmake)
ul_include(githooks.cmake)

ul_include(googletest.cmake)

add_subdirectory(sdks)
add_subdirectory(gui/Qt/qt_qml_extensions) # for test app

ul_include(qt.cmake)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Svg)
qt_standard_project_setup(REQUIRES 6.8)

file(GLOB_RECURSE targetAsLibForTest_SRC_INCS "src/*.h*")

set(targetAsLibForTest_IMPL_SOURCES
    src/_gui/src_Qt/resource_string/resource_string.cpp
)

set(targetAsLibForTest_SOURCES
    src/_gui/interface/resource_string.cpp

   ${targetAsLibForTest_IMPL_SOURCES}
)

set(targetAsLibForTest ${PROJECT_NAME}AsLibForTest)

qt_add_library(${targetAsLibForTest} STATIC
    ${targetAsLibForTest_SOURCES}
    ${targetAsLibForTest_SRC_INCS}
)

configure_file(${target}_build_config.h.in ${CMAKE_BINARY_DIR}/${target}_build_config.h)
target_include_directories(${targetAsLibForTest} PUBLIC src)

target_link_libraries(${targetAsLibForTest} PUBLIC mb-util Qt::Core Qt::Gui Qt::Quick Qt::QuickControls2 Qt::Svg)

ul_set_target_defaults(${targetAsLibForTest})

set(${targetAsLibForTest}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${targetAsLibForTest}: include directories" FORCE)


###############################################################################################

# Only purpose of this so far: prevent showing an additional console window in Qt apps (happens for msvc in way
# impossible to overlook, but also for mingw builds when not starting from IDE).
if (WIN32)
    set(WIN32_ON_OFF_SWITCH "WIN32")
else ()
    set(WIN32_ON_OFF_SWITCH "")
endif ()
set(target_IMPL_SOURCES
    src/_gui/src_Qt/main.cpp
)

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
    MACOSX_BUNDLE
    ${target_UWP_ASSETS}
    ${target_IMPL_SOURCES}
)

set(target_QML_DIR ${CMAKE_SOURCE_DIR}/gui/Qt)

qt_add_executable(${target} ${target_input})

if (UL_ANDROID)
    set_property(TARGET ${target} PROPERTY QT_ANDROID_TARGET_SDK_VERSION ${QT_ANDROID_TARGET_SDK_VERSION})
    set_property(TARGET ${target} PROPERTY QT_ANDROID_MIN_SDK_VERSION ${QT_ANDROID_MIN_SDK_VERSION})
endif ()

qt_add_qml_module(${target}
    URI ${target}
    NO_RESOURCE_TARGET_PATH
    QML_FILES
        "gui/Qt/MinimalQtTestApp/cfg/CfgProgMain.qml"
        "gui/Qt/MinimalQtTestApp/AppWindow.qml"
        "gui/Qt/qt_qml_extensions/_shared/CustomScaler.qml"
        "gui/Qt/qt_qml_extensions/colors_palette/ExtColors.qml"
        "gui/Qt/qt_qml_extensions/colors_palette/ExtColorsTest.qml"
        "gui/Qt/qt_qml_extensions/colors_palette/impl/ColorsActive.qml"
        "gui/Qt/qt_qml_extensions/colors_palette/impl/ColorsDisabled.qml"
        "gui/Qt/qt_qml_extensions/colors_palette/impl/ColorsInactive.qml"
        "gui/Qt/qt_qml_extensions/colors_palette/impl/TestRectangle.qml"
        "gui/Qt/qt_qml_extensions/controls/_shared/impl/CfgControls.qml"
        "gui/Qt/qt_qml_extensions/controls/_shared/impl/obtainButtonProps.js"
        "gui/Qt/qt_qml_extensions/controls/_shared/impl/obtainControlProps.js"
        "gui/Qt/qt_qml_extensions/controls/_shared/impl/obtainListDelegateProps.js"
        "gui/Qt/qt_qml_extensions/controls/ControlsTest.qml"
        "gui/Qt/qt_qml_extensions/controls/ExtControlsCfg.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ApplicationWindow_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Button_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ButtonTool_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ButtonWithIcon_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/CharColorSwitch_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/CheckBox_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ComboBox_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Drawer_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Frame_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/GroupBox_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/impl/ButtonBackground.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/impl/FrameBackground.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/impl/PaneBackground.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/impl/TextFieldBackground.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Label_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ListBox_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Pane_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Popup_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/RadioButton_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ScrollBar_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ShortcutLabel_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Spacer_Col_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/Spacer_Row_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/SpinBox_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/SpinBoxLabelRow_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TabBar_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TabButton_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TextArea_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TextAreaScrollable_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TextField_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TextFieldBrowseBtnRow_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TextFieldWithLabel_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/TextInput_.qml"
        "gui/Qt/qt_qml_extensions/controls/minimal/ToolTip_.qml"
        "gui/Qt/qt_qml_extensions/controls/TestHelperBox.qml"
        "gui/Qt/qt_qml_extensions/controls/TestHelperCornerMarkers.qml"
        "gui/Qt/qt_qml_extensions/core/noop.js"
        "gui/Qt/qt_qml_extensions/core/os.js"
        "gui/Qt/qt_qml_extensions/fonts/ExtFont.qml"
        "gui/Qt/qt_qml_extensions/fonts/ExtFontMetrics.qml"
        "gui/Qt/qt_qml_extensions/fonts/ExtFontTest.qml"
        "gui/Qt/qt_qml_extensions/fonts/impl/TestRectangle.qml"
        "gui/Qt/qt_qml_extensions/fonts/schemes/Def.qml"
        "gui/Qt/qt_qml_extensions/fonts/schemes/DefQC2.qml"
        "gui/Qt/qt_qml_extensions/fonts/schemes/Mistral.qml"
        "gui/Qt/qt_qml_extensions/layout/ExtLayoutTest.qml"
        "gui/Qt/qt_qml_extensions/layout/ExtScale.qml"
        "gui/Qt/qt_qml_extensions/layout/ExtSpacing.qml"
        "gui/Qt/qt_qml_extensions/layout/impl/TestHelperObj.qml"
        "gui/Qt/qt_qml_extensions/layout/impl/TestRectangle.qml"
        "gui/Qt/qt_qml_extensions/testapp/gui/AppWindow.qml"
        "gui/Qt/qt_qml_extensions/testapp/gui/Draft.qml"
        "gui/Qt/qt_qml_extensions/testapp/gui/dummydata/cfgModel.qml"
    RESOURCES
        "gui/Qt/qt_qml_extensions/colors_palette/impl/qmldir"
        "gui/Qt/qt_qml_extensions/controls/_shared/impl/checkmark.png"
        "gui/Qt/qt_qml_extensions/controls/_shared/impl/delete_cross.svg"
        "gui/Qt/qt_qml_extensions/controls/_shared/impl/open.png"
        "gui/Qt/qt_qml_extensions/controls/menu.svg"
        "gui/Qt/qt_qml_extensions/controls/spheres.svg"
)

target_link_libraries(${target} PRIVATE ${targetAsLibForTest})

ul_set_target_defaults(${target})

ul_include(qt_deploy.cmake)
ul_qt_deploy(${target} ${target_QML_DIR})

if (ANDROID)
    set(RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR "${CMAKE_CURRENT_LIST_DIR}/res/android")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory "${RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory "installer/_content_for_bindir/_ico/android"
        "${RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}/res"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    file(GLOB package_sources
        "${UL_CMAKE_UTIL_DIR}/android/*"
    )
    file(COPY ${package_sources} DESTINATION "${RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}")
    configure_file(
        "${RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}/AndroidManifest.xml.in"
        "${RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}/AndroidManifest.xml"
        @ONLY
    )
    set_target_properties(${target} PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR "${RUNTIME_OUTPUT_DIRECTORY_PACKAGESUBDIR}"
    )
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${target} PRIVATE -Wno-missing-prototypes)
endif()

if (UNIX AND NOT APPLE AND CMAKE_CROSSCOMPILING)
    find_package(Qt6 REQUIRED COMPONENTS QuickTemplates2)

    # Work around QTBUG-86533
    target_link_libraries(${target} PRIVATE Qt6::QuickTemplates2)
endif()

install(TARGETS ${target}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET ${target}
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)
install(SCRIPT ${deploy_script})


################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE targetTest_IMPL_SOURCES
    "src/_gui/src_Qt/*.test.cpp"
    )

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
