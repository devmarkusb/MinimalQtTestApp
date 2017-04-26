#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.load(QUrl("qrc:/MinimalQtTestApp/AppWindow.qml"));
    return app.exec();
}
