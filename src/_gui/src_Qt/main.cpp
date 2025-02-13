#include "ul/ul.h"

#include "ul/warnings.h"
UL_PRAGMA_WARNINGS_PUSH_AND_DISABLE_ALL_MSVC
#include <QGuiApplication>
#include <QQmlApplicationEngine>
UL_PRAGMA_WARNINGS_POP

int main(int argc, char* argv[]) {
    ul::tracer::init();

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.load(QUrl("qrc:/MinimalQtTestApp/AppWindow.qml"));

    return QGuiApplication::exec();
}
