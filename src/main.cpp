#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Kirill Maximov");
    app.setOrganizationDomain("none");
    QQmlApplicationEngine engine;
    BackEnd *backEnd(new BackEnd(engine));

    engine.rootContext()->setContextProperty("backEnd", backEnd);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    engine.addImageProvider("imageProvider", backEnd);

    return app.exec();
}
