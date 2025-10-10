#include "filehandler.h"
#include "Math/Scene.h"

#include "Math/ValueTracker/trackermanager.h"

FileHandler::FileHandler(QObject *parent)
    : QObject(parent)
{
}

bool FileHandler::saveScene(Scene *scene, const QString &filePath)
{
    qDebug() << "FileHandler::saveScene called with filePath:" << filePath;

    if (!scene)
    {
        qDebug() << "Error: No scene provided";
        emit saveCompleted(false, "No scene to save");
        return false;
    }

    QJsonObject rootObject;

    // Add format version for future compatibility
    rootObject["version"] = "1.0";
    rootObject["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    emit saveProgress(10);

    // Save scene metadata
    rootObject["scene"] = scene->getData().toJson().object();
    // qDebug() << "Saving scene metadata...";
    // rootObject["scene"] = getSceneMetadata(scene);
    // emit saveProgress(25);

    // // Save mobjects
    // qDebug() << "Saving mobjects metadata...";
    // rootObject["mobjects"] = getMobjectsMetadata(scene);
    // emit saveProgress(50);

    // // Save animations
    // qDebug() << "Saving animations metadata...";
    // rootObject["animations"] = getAnimationsMetadata(scene);
    // emit saveProgress(70);

    // // Save parser state
    // qDebug() << "Saving parser metadata...";
    // rootObject["parser"] = getParserMetadata(scene);
    // emit saveProgress(85);

    // // Save tracker state
    // qDebug() << "Saving tracker metadata...";
    // rootObject["trackers"] = getTrackersMetadata(scene);
    // emit saveProgress(95);

    // // Write to file
    qDebug() << "Writing JSON to file:" << filePath;
    QJsonDocument doc(rootObject);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        QString errorMsg = "Could not open file for writing: " + file.errorString();
        qDebug() << "Error:" << errorMsg;
        emit saveCompleted(false, errorMsg);
        return false;
    }

    qint64 bytesWritten = file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Successfully wrote" << bytesWritten << "bytes to file";
    emit saveProgress(100);
    emit saveCompleted(true, "Scene saved successfully");
    return true;
}

bool FileHandler::loadScene(Scene *scene, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit loadCompleted(false, "Could not open file for reading: " + file.errorString());
        return false;
    }

    emit loadProgress(10);

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull())
    {
        emit loadCompleted(false, "Invalid JSON file");
        return false;
    }

    QJsonObject rootObject = doc.object();

    // Check version compatibility
    QString version = rootObject["version"].toString();
    if (version.isEmpty())
    {
        qWarning() << "No version info in file, proceeding with caution";
    }

    emit loadProgress(25);

    // Clear current scene
    if (scene)
    {
        // Remove all mobjects
        for (const QString &id : scene->getAllMobjectIds())
        {
            scene->removeMobject(id);
        }
        // Clear parser variables
        if (rootObject.contains("scene")){
            qInfo()<<"SCENDATA SET CALING";
            scene->setFromJSON(rootObject["scene"].toObject());
            qInfo()<<"SCENDATA AVAILABLE";
        }
    }

    // emit loadProgress(40);

    // // Recreate scene
    // if (!recreateScene(scene, rootObject["scene"].toObject()))
    // {
    //     emit loadCompleted(false, "Failed to recreate scene");
    //     return false;
    // }

    // emit loadProgress(55);

    // // Recreate mobjects
    // if (!recreateMobjects(scene, rootObject["mobjects"].toArray()))
    // {
    //     emit loadCompleted(false, "Failed to recreate mobjects");
    //     return false;
    // }

    // emit loadProgress(70);

    // // Recreate parser state
    // if (!recreateParserState(scene, rootObject["parser"].toObject()))
    // {
    //     emit loadCompleted(false, "Failed to recreate parser state");
    //     return false;
    // }

    // emit loadProgress(85);

    // // Recreate tracker state
    // if (!recreateTrackerState(scene, rootObject["trackers"].toObject()))
    // {
    //     emit loadCompleted(false, "Failed to recreate tracker state");
    //     return false;
    // }

    // emit loadProgress(95);

    // // Recreate animations (last because they depend on mobjects)
    // if (!recreateAnimations(scene, rootObject["animations"].toObject()))
    // {
    //     emit loadCompleted(false, "Failed to recreate animations");
    //     return false;
    // }

    // emit loadProgress(100);
    // emit loadCompleted(true, "Scene loaded successfully");
    return true;
}
