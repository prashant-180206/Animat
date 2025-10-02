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

private:
    // Helper method to update mobject hash with correct ID

signals:
    void saveCompleted(bool success, const QString &message);
    void loadCompleted(bool success, const QString &message);
    void saveProgress(int percentage);
    void loadProgress(int percentage);
};

#endif // FILEHANDLER_H
