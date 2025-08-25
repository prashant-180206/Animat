#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Math/scene.h"
#include "Singleton.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Scene>("Animat",1,0,"Scene");
    qmlRegisterSingletonType<ConstantsSingleton>("Animat", 1, 0, "Constants", [](QQmlEngine*, QJSEngine*) -> QObject* {
        return new ConstantsSingleton();
    });

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Animat", "Main");

    return app.exec();
}
