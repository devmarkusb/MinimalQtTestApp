#include "toolib/date_time/date_time.h"
#include "toolib/ptr2ref.h"
#include "toolib/trace.h"
#include "uiwrap/filesys/filesys.h"
#include "toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <fstream>


namespace os
{
uiw::IFileSys& filesys() { return too::Ptr2Ref(uiw::IFileSys::getInstance()); }
} // os

namespace test
{
// for testing, whether winrt app package will contain the needed dll dependency (turns out it does)
void pullInRuntimeDependency()
{
    std::pair<too::date_time::Years, too::date_time::Months> ym{1, 14};
    too::date_time::normalize::do_it(ym);
    too::trace("INFO") << ym.first << ", " << ym.second;
}
} // test

int main(int argc, char* argv[])
{
    too::tracer::init();

    // block for testing file writing and interface access (had some strange problems on Android)
    std::ofstream f{"testfile.txt"};
    f << "start" << std::endl;
    std::string s;
    s = os::filesys().getSystemPath(uiw::IFileSys::ESysPathType::APPDATA_writable, true);
    f << "str" << std::endl;
    f << s << std::endl;
    f << "/str" << std::endl;

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.load(QUrl("qrc:/MinimalQtTestApp/AppWindow.qml"));

    test::pullInRuntimeDependency();

    return app.exec();
}
