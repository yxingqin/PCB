#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Simulator.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication::setOrganizationName("yxqin");
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(".");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl)
        {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("$Simulator",Simulator::getInstence());
    engine.load(url);
    Simulator::setWindow(engine.rootObjects().first());
    int ret = app.exec();
    Simulator::destroy();
    return ret;
}