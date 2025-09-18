#include <QGuiApplication>
#include <QQmlApplicationEngine>


#include "Math/scene.h"
#include "Utils/Singleton.h"
#include "Math/Mobjects/Mobject.h"
#include "Utils/mobjectregistry.h"
#include "Math/ValueTracker/valuetracker.h"
#include "Math/ValueTracker/ptvaluetracker.h"
#include "Math/ValueTracker/playbackslider.h"
#include "trackermanager.h"
#include <QQmlContext>

// #include "muParser.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Scene>("Animat",1,0,"Scene");
    qmlRegisterType<MobjectRegistry>("Animat", 1, 0, "MobjectRegistry");
    qmlRegisterType<MProperties>("Animat", 1, 0, "MProperties");
    qmlRegisterType<ValueTracker>("Animat", 1, 0, "ValueTracker");
    qmlRegisterType<PtValueTracker>("Animat", 1, 0, "PtValueTracker");
    qmlRegisterType<PlaybackSlider>("Animat", 1, 0, "PlaybackSlider");
    qmlRegisterType<AnimationManager>("Animat", 1, 0, "AnimManager");
    qmlRegisterType<AnimPacket>("Animat", 1, 0, "AnimPacket");
    qmlRegisterType<TrackerManager>("Animat", 1, 0, "TrackerManager");


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
