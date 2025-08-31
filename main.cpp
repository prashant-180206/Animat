#include <QGuiApplication>
#include <QQmlApplicationEngine>


#include "Math/Mobjects/MRectangle.h"
#include "Math/scene.h"
#include "Utils/Singleton.h"
#include "Math/Mobjects/Mobject.h"
#include "Utils/mobjectregistry.h"
#include <QQmlContext>
// #include "muParser.h"




int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Scene>("Animat",1,0,"Scene");
    qmlRegisterType<MRectangle>("Animat",1,0,"MRectangle");
    qmlRegisterType<MobjectRegistry>("Animat", 1, 0, "MobjectRegistry");


    qmlRegisterSingletonType<ConstantsSingleton>("Animat", 1, 0, "Constants", [](QQmlEngine*, QJSEngine*) -> QObject* {
        return new ConstantsSingleton();
    });

    QQmlApplicationEngine engine;

    // MobjectRegistry modal;
    // engine.rootContext()->setContextProperty("Mobjectregistry", &modal);

    // engine.load(QUrl(QStringLiteral("Main.qml")));



    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Animat", "Main");

    return app.exec();
}
