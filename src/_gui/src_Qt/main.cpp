#include "toolib/date_time/date_time.h"
#include "uiwrap/filesys/filesys.h"
#include "ul/ul.h"

#include "ul/warnings.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include <QGuiApplication>
#include <QQmlApplicationEngine>
UL_PRAGMA_WARNINGS_POP

#include <fstream>
#include <string>

namespace os
{
namespace
{
uiw::IFileSys& filesys()
{
    return ul::ptr2Ref(uiw::IFileSys::getInstance());
}
} // namespace
} // namespace os

namespace test
{
namespace
{
// for testing, whether winrt app package will contain the needed dll dependency (turns out it does)
void pullInRuntimeDependency()
{
    std::pair<too::date_time::Years, too::date_time::Months> ym{1, 14};
    too::date_time::normalize::do_it(ym);
    ul::trace("INFO") << ym.first << ", " << ym.second;
}
} // namespace
} // namespace test

int main(int argc, char* argv[])
{
    ul::tracer::init();

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

    return QGuiApplication::exec();
}
