import "../qt_qml_extensions/colors_palette"
import "../qt_qml_extensions/controls"
import "../qt_qml_extensions/controls/minimal"
import "../qt_qml_extensions/fonts"
import "../qt_qml_extensions/layout"
import "cfg"
import QtQuick 2.8
import QtQuick.Window 2.2


ApplicationWindow_ {
    id: rootAppWindow

    ExtColors {
        id: extColors // don't change this specifier!
        schemeChoice: "bright"
    }

    ExtFont {
        id: extFont // don't change this specifier!
        schemeChoice: "bright"
    }

    ExtFontMetrics {
        id: extFontMetrics // don't change this specifier!
    }

    ExtSpacing {
        id: extSpacing // don't change this specifier!
    }

    ExtControlsCfg {
        id: extControlsCfg // don't change this specifier!
    }

    CfgProgMain {
        id: cfgProgMain
    }

    visible: true

    minimumWidth: cfgProgMain.wProgramWidth_min
    minimumHeight: cfgProgMain.hProgramHeight_min
    width: cfgProgMain.wProgramWidth_def
    height: cfgProgMain.hProgramHeight_def

    title: "test app"

    Component.onCompleted: {
        console.time("onCompleted took")
        console.count("rootAppWindow Component.onCompleted entered")
        console.trace()

        console.timeEnd("onCompleted took")
        console.log("rootAppWindow Component.onCompleted exiting...")
    }

    onVisibleChanged: {
        console.log("rootAppWindow onVisibleChanged, visible:", visible)
    }

    Component.onDestruction: {
        console.log("rootAppWindow Component.onDestruction entered")
        // note: cpp view models are probably already dead at this stage
        console.log("rootAppWindow Component.onDestruction exiting...")
    }

    onClosing: {
        console.log("rootAppWindow onClosing entered")
        console.log("rootAppWindow onClosing exiting...")
    }


    // for experimenting
    //Draft {}
}
