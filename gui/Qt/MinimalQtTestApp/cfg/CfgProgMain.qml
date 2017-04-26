//! Program's main window/dialog initial settings. QML only. Constants almost exlusively.
//! Don't mix up with cfgProgMainModel which exchanges potentially variable data with the cpp sources.
/** Usage:
    Put the following code right somewhere in your root application item
    \code
    CfgProgMain {
        id: cfgProgMain
    }
    \endcode
    Then you can refer to your global settings via e.g. cfgProgMain.wProgramWidth_def from everywhere below.
*/
import QtQuick 2.8


QtObject {
    //####################################################################################################################
    // main window
    
    // Note that we choose width > height, since this should be mostz convenient screen orientation for this app.
    readonly property real wProgramWidth_def: 640 // client size for Windows desktop (without title bar)
    readonly property real hProgramHeight_def: 480 // "
    readonly property real wProgramWidth_min: 480//320
    readonly property real hProgramHeight_min: 320//240
}
