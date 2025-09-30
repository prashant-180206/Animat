#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVariantMap>
#include <QPointF>
#include <QColor>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <qqmlintegration.h>

class Scene;
class ClickableMobject;
class AnimPacket;
class AnimationManager;
class Parser;
class TrackerManager;
class MProperties;

class FileHandler : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit FileHandler(QObject *parent = nullptr);

    // Main save/load methods
    Q_INVOKABLE bool saveScene(Scene *scene, const QString &filePath);
    Q_INVOKABLE bool loadScene(Scene *scene, const QString &filePath);

    // Metadata extraction methods
    QJsonObject getSceneMetadata(Scene *scene);
    QJsonArray getMobjectsMetadata(Scene *scene);
    QJsonObject getMobjectMetadata(ClickableMobject *mobject);
    QJsonObject getAnimationsMetadata(Scene *scene);
    QJsonObject getParserMetadata(Scene *scene);
    QJsonObject getTrackersMetadata(Scene *scene);

    // Properties extraction methods
    QJsonObject extractMProperties(MProperties *properties);
    QJsonObject extractBaseProperties(MProperties *properties);
    QJsonObject extractCircleProperties(MProperties *properties);
    QJsonObject extractLineProperties(MProperties *properties);
    QJsonObject extractPolygonProperties(MProperties *properties);
    QJsonObject extractTextProperties(MProperties *properties);
    QJsonObject extractCurveProperties(MProperties *properties);

    // Scene recreation methods
    bool recreateScene(Scene *scene, const QJsonObject &sceneData);
    bool recreateMobjects(Scene *scene, const QJsonArray &mobjectsData);
    bool recreateAnimations(Scene *scene, const QJsonObject &animationsData);
    bool recreateParserState(Scene *scene, const QJsonObject &parserData);
    bool recreateTrackerState(Scene *scene, const QJsonObject &trackersData);

    // Utility methods
    QJsonObject pointToJson(const QPointF &point);
    QPointF jsonToPoint(const QJsonObject &pointJson);
    QJsonObject colorToJson(const QColor &color);
    QColor jsonToColor(const QJsonObject &colorJson);

private:
    // Helper methods for animation packet serialization
    QJsonObject animPacketToJson(AnimPacket *packet);
    bool jsonToAnimPacket(const QJsonObject &packetJson, AnimPacket *packet);

    // Helper method to update mobject hash with correct ID
    void updateMobjectId(Scene *scene, const QString &oldId, const QString &newId);

signals:
    void saveCompleted(bool success, const QString &message);
    void loadCompleted(bool success, const QString &message);
    void saveProgress(int percentage);
    void loadProgress(int percentage);
};

#endif // FILEHANDLER_H