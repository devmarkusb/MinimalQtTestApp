#include "Toolib/date_time/date_time.h"
#include "Toolib/trace.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"

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

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.load(QUrl("qrc:/MinimalQtTestApp/AppWindow.qml"));

    test::pullInRuntimeDependency();

    return app.exec();
}
