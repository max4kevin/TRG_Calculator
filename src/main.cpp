#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    BackEnd *backEnd(new BackEnd(app));
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("backEnd", backEnd);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    engine.addImageProvider("imageProvider", backEnd);

    return app.exec();
}
