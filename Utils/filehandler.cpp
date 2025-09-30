#include "filehandler.h"
#include "Math/Scene.h"
#include "Math/Helper/ClickableMobject.h"
#include "Math/Helper/mproperties.h"
#include "Math/Helper/Properties/baseproperties.h"
#include "Math/Animations/animationmanager.h"
#include "Math/Animations/animpacket.h"
#include "Math/Parser/parser.h"
#include "Math/Parser/trackermanager.h"

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
    qDebug() << "Saving scene metadata...";
    rootObject["scene"] = getSceneMetadata(scene);
    emit saveProgress(25);

    // Save mobjects
    qDebug() << "Saving mobjects metadata...";
    rootObject["mobjects"] = getMobjectsMetadata(scene);
    emit saveProgress(50);

    // Save animations
    qDebug() << "Saving animations metadata...";
    rootObject["animations"] = getAnimationsMetadata(scene);
    emit saveProgress(70);

    // Save parser state
    qDebug() << "Saving parser metadata...";
    rootObject["parser"] = getParserMetadata(scene);
    emit saveProgress(85);

    // Save tracker state
    qDebug() << "Saving tracker metadata...";
    rootObject["trackers"] = getTrackersMetadata(scene);
    emit saveProgress(95);

    // Write to file
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
        scene->clearParserVariables();
    }

    emit loadProgress(40);

    // Recreate scene
    if (!recreateScene(scene, rootObject["scene"].toObject()))
    {
        emit loadCompleted(false, "Failed to recreate scene");
        return false;
    }

    emit loadProgress(55);

    // Recreate mobjects
    if (!recreateMobjects(scene, rootObject["mobjects"].toArray()))
    {
        emit loadCompleted(false, "Failed to recreate mobjects");
        return false;
    }

    emit loadProgress(70);

    // Recreate parser state
    if (!recreateParserState(scene, rootObject["parser"].toObject()))
    {
        emit loadCompleted(false, "Failed to recreate parser state");
        return false;
    }

    emit loadProgress(85);

    // Recreate tracker state
    if (!recreateTrackerState(scene, rootObject["trackers"].toObject()))
    {
        emit loadCompleted(false, "Failed to recreate tracker state");
        return false;
    }

    emit loadProgress(95);

    // Recreate animations (last because they depend on mobjects)
    if (!recreateAnimations(scene, rootObject["animations"].toObject()))
    {
        emit loadCompleted(false, "Failed to recreate animations");
        return false;
    }

    emit loadProgress(100);
    emit loadCompleted(true, "Scene loaded successfully");
    return true;
}

QJsonObject FileHandler::getSceneMetadata(Scene *scene)
{
    QJsonObject sceneObj;
    if (!scene)
        return sceneObj;

    sceneObj["width"] = scene->width();
    sceneObj["height"] = scene->height();
    sceneObj["backgroundColor"] = colorToJson(scene->getbg());
    sceneObj["borderColor"] = colorToJson(scene->getBorderColor());
    sceneObj["scaleFactor"] = scene->scalefactor();
    sceneObj["activeId"] = scene->activeId();

    return sceneObj;
}

QJsonArray FileHandler::getMobjectsMetadata(Scene *scene)
{
    QJsonArray mobjects;
    if (!scene)
        return mobjects;

    for (const QString &id : scene->getAllMobjectIds())
    {
        ClickableMobject *mobject = scene->getMobject(id);
        if (mobject)
        {
            mobjects.append(getMobjectMetadata(mobject));
        }
    }

    return mobjects;
}

QJsonObject FileHandler::getMobjectMetadata(ClickableMobject *mobject)
{
    QJsonObject obj;
    if (!mobject)
        return obj;

    obj["id"] = mobject->getId();
    obj["center"] = pointToJson(mobject->getCenter());
    obj["width"] = mobject->width();
    obj["height"] = mobject->height();
    obj["z"] = mobject->z();
    obj["visible"] = mobject->isVisible();
    obj["enabled"] = mobject->isEnabled();

    // Save all properties
    MProperties *props = mobject->getProperties();
    if (props)
    {
        obj["properties"] = extractMProperties(props);
    }

    return obj;
}

QJsonObject FileHandler::getAnimationsMetadata(Scene *scene)
{
    QJsonObject animations;
    if (!scene)
        return animations;

    AnimationManager *animator = scene->animator();
    if (!animator)
        return animations;

    QJsonArray packets;
    for (AnimPacket *packet : animator->packets())
    {
        if (packet)
        {
            packets.append(animPacketToJson(packet));
        }
    }

    animations["packets"] = packets;
    animations["currentPacketIndex"] = animator->getCurrentIndex();

    return animations;
}

QJsonObject FileHandler::getParserMetadata(Scene *scene)
{
    QJsonObject parser;
    if (!scene)
        return parser;

    Parser *p = scene->parser();
    if (!p)
        return parser;

    // Save all variables (we need to get them through evaluation)
    QJsonObject variables;

    // Note: Parser class might need to expose a getVariables() method
    // For now, we'll save what we can access
    parser["variables"] = variables;

    return parser;
}

QJsonObject FileHandler::getTrackersMetadata(Scene *scene)
{
    QJsonObject trackers;
    if (!scene)
        return trackers;

    TrackerManager *tm = scene->trackers();
    if (!tm)
        return trackers;

    QJsonArray trackerNames;

    // Use Scene's getTrackerNames method which gets all trackers
    QStringList allTrackerNames = scene->getTrackerNames();
    for (const QString &name : allTrackerNames)
    {
        QJsonObject tracker;
        tracker["name"] = name;
        tracker["value"] = scene->getTrackerValue(name);
        tracker["point"] = pointToJson(scene->getTrackerPoint(name));
        trackerNames.append(tracker);
    }

    trackers["trackers"] = trackerNames;

    return trackers;
}

QJsonObject FileHandler::extractMProperties(MProperties *properties)
{
    QJsonObject props;
    if (!properties)
        return props;

    props["base"] = extractBaseProperties(properties);
    props["circle"] = extractCircleProperties(properties);
    props["line"] = extractLineProperties(properties);
    props["polygon"] = extractPolygonProperties(properties);
    props["text"] = extractTextProperties(properties);
    props["curve"] = extractCurveProperties(properties);

    return props;
}

QJsonObject FileHandler::extractBaseProperties(MProperties *properties)
{
    QJsonObject base;
    if (!properties || !properties->base())
        return base;

    auto *bp = properties->base();
    base["name"] = bp->name();
    base["pos"] = pointToJson(bp->pos());
    base["size"] = pointToJson(bp->size());
    base["color"] = colorToJson(bp->color());
    base["opacity"] = bp->opacity();
    base["type"] = bp->type();
    base["scale"] = bp->scale();
    base["rotation"] = bp->rotation();
    base["zindex"] = bp->zindex();

    return base;
}

QJsonObject FileHandler::extractCircleProperties(MProperties *properties)
{
    QJsonObject circle;
    if (!properties || !properties->circle())
        return circle;

    auto *cp = properties->circle();
    // Add circle-specific properties here when available
    // circle["radius"] = cp->radius();

    return circle;
}

QJsonObject FileHandler::extractLineProperties(MProperties *properties)
{
    QJsonObject line;
    if (!properties || !properties->line())
        return line;

    auto *lp = properties->line();
    // Add line-specific properties here when available
    // line["strokeWidth"] = lp->strokeWidth();

    return line;
}

QJsonObject FileHandler::extractPolygonProperties(MProperties *properties)
{
    QJsonObject polygon;
    if (!properties || !properties->polygon())
        return polygon;

    auto *pp = properties->polygon();
    // Add polygon-specific properties here when available

    return polygon;
}

QJsonObject FileHandler::extractTextProperties(MProperties *properties)
{
    QJsonObject text;
    if (!properties || !properties->text())
        return text;

    auto *tp = properties->text();
    // Add text-specific properties here when available
    // text["content"] = tp->content();
    // text["fontSize"] = tp->fontSize();

    return text;
}

QJsonObject FileHandler::extractCurveProperties(MProperties *properties)
{
    QJsonObject curve;
    if (!properties || !properties->curve())
        return curve;

    auto *cp = properties->curve();
    // Add curve-specific properties here when available

    return curve;
}

// Recreation methods
bool FileHandler::recreateScene(Scene *scene, const QJsonObject &sceneData)
{
    if (!scene || sceneData.isEmpty())
        return false;

    scene->setWidth(sceneData["width"].toDouble());
    scene->setHeight(sceneData["height"].toDouble());
    scene->setbg(jsonToColor(sceneData["backgroundColor"].toObject()));
    scene->setActiveId(sceneData["activeId"].toString());

    return true;
}

bool FileHandler::recreateMobjects(Scene *scene, const QJsonArray &mobjectsData)
{
    if (!scene)
        return false;

    for (const auto &value : mobjectsData)
    {
        QJsonObject mobjectData = value.toObject();
        QString type = mobjectData["properties"].toObject()["base"].toObject()["type"].toString();

        if (type.isEmpty())
            continue;

        // Create the mobject using the MobjectMap
        scene->add_mobject(type);

        // Get the newly created mobject and update its properties
        QStringList ids = scene->getAllMobjectIds();
        if (!ids.isEmpty())
        {
            QString newId = ids.last();
            ClickableMobject *mobject = scene->getMobject(newId);

            if (mobject)
            {
                // Restore the original ID
                QString originalId = mobjectData["id"].toString();
                updateMobjectId(scene, newId, originalId);
                mobject->setId(originalId);

                // Restore position and properties
                QPointF center = jsonToPoint(mobjectData["center"].toObject());
                mobject->setCenter(center.x(), center.y());
                mobject->setZ(mobjectData["z"].toDouble());
                mobject->setVisible(mobjectData["visible"].toBool());
                mobject->setEnabled(mobjectData["enabled"].toBool());

                // Restore properties
                QJsonObject propsData = mobjectData["properties"].toObject();
                if (!propsData.isEmpty())
                {
                    MProperties *props = mobject->getProperties();
                    if (props)
                    {
                        // Restore base properties
                        QJsonObject baseData = propsData["base"].toObject();
                        if (props->base() && !baseData.isEmpty())
                        {
                            props->base()->setName(baseData["name"].toString());
                            props->base()->setPos(jsonToPoint(baseData["pos"].toObject()));
                            props->base()->setSize(jsonToPoint(baseData["size"].toObject()));
                            props->base()->setColor(jsonToColor(baseData["color"].toObject()));
                            props->base()->setOpacity(baseData["opacity"].toDouble());
                            props->base()->setType(baseData["type"].toString());
                            props->base()->setScale(baseData["scale"].toDouble());
                            props->base()->setRotation(baseData["rotation"].toDouble());
                            props->base()->setZindex(baseData["zindex"].toDouble());
                        }

                        // Restore other property types as needed...
                    }
                }
            }
        }
    }

    return true;
}

bool FileHandler::recreateAnimations(Scene *scene, const QJsonObject &animationsData)
{
    if (!scene)
        return false;

    AnimationManager *animator = scene->animator();
    if (!animator)
        return false;

    QJsonArray packetsData = animationsData["packets"].toArray();

    for (const auto &value : packetsData)
    {
        QJsonObject packetData = value.toObject();

        // Create new packet
        AnimPacket *packet = new AnimPacket(animator);

        if (jsonToAnimPacket(packetData, packet))
        {
            // Add packet to animator (might need to expose this method)
            // animator->addPacket(packet);
        }
        else
        {
            delete packet;
        }
    }

    // Restore current packet index
    int currentIndex = animationsData["currentPacketIndex"].toInt();
    animator->jumpToIndex(currentIndex);

    return true;
}

bool FileHandler::recreateParserState(Scene *scene, const QJsonObject &parserData)
{
    if (!scene)
        return false;

    Parser *parser = scene->parser();
    if (!parser)
        return false;

    // Clear existing variables
    scene->clearParserVariables();

    // Restore variables
    QJsonObject variables = parserData["variables"].toObject();
    for (auto it = variables.begin(); it != variables.end(); ++it)
    {
        scene->setParserVariable(it.key(), it.value().toDouble());
    }

    return true;
}

bool FileHandler::recreateTrackerState(Scene *scene, const QJsonObject &trackersData)
{
    if (!scene)
        return false;

    TrackerManager *tm = scene->trackers();
    if (!tm)
        return false;

    QJsonArray trackersArray = trackersData["trackers"].toArray();

    for (const auto &value : trackersArray)
    {
        QJsonObject trackerData = value.toObject();
        QString name = trackerData["name"].toString();
        double val = trackerData["value"].toDouble();
        QPointF point = jsonToPoint(trackerData["point"].toObject());

        // Use Scene's executeTrackerScript method to recreate trackers
        // This would require creating appropriate tracker creation scripts
        // For now, we'll leave this as a placeholder for future implementation

        // Example script format might be:
        // QString script = QString("create_tracker %1 = %2").arg(name).arg(val);
        // scene->executeTrackerScript(script);
    }

    return true;
}

// Utility methods
QJsonObject FileHandler::pointToJson(const QPointF &point)
{
    QJsonObject obj;
    obj["x"] = point.x();
    obj["y"] = point.y();
    return obj;
}

QPointF FileHandler::jsonToPoint(const QJsonObject &pointJson)
{
    return QPointF(pointJson["x"].toDouble(), pointJson["y"].toDouble());
}

QJsonObject FileHandler::colorToJson(const QColor &color)
{
    QJsonObject obj;
    obj["r"] = color.red();
    obj["g"] = color.green();
    obj["b"] = color.blue();
    obj["a"] = color.alpha();
    return obj;
}

QColor FileHandler::jsonToColor(const QJsonObject &colorJson)
{
    return QColor(
        colorJson["r"].toInt(),
        colorJson["g"].toInt(),
        colorJson["b"].toInt(),
        colorJson["a"].toInt());
}

QJsonObject FileHandler::animPacketToJson(AnimPacket *packet)
{
    QJsonObject obj;
    if (!packet)
        return obj;

    obj["name"] = packet->name();
    obj["description"] = packet->description();
    obj["animationType"] = packet->animationType();
    obj["startTime"] = packet->startTime();
    obj["duration"] = packet->duration();

    // Save animations within the packet
    QJsonArray animations;
    // Note: AnimPacket might need to expose its animations list
    obj["animations"] = animations;

    return obj;
}

bool FileHandler::jsonToAnimPacket(const QJsonObject &packetJson, AnimPacket *packet)
{
    if (!packet)
        return false;

    packet->setName(packetJson["name"].toString());
    packet->setDescription(packetJson["description"].toString());
    packet->setStartTime(packetJson["startTime"].toDouble());

    // Restore animations
    QJsonArray animations = packetJson["animations"].toArray();
    for (const auto &value : animations)
    {
        QJsonObject animData = value.toObject();
        // Create and add animations to packet
        // This requires access to Animation creation methods
    }

    return true;
}

void FileHandler::updateMobjectId(Scene *scene, const QString &oldId, const QString &newId)
{
    // This is a helper method that would need access to Scene's m_objects hash
    // For now, we'll leave this as a placeholder since we can't directly modify
    // the private member. The Scene class would need to provide a method like:
    // scene->updateMobjectId(oldId, newId);
    Q_UNUSED(scene)
    Q_UNUSED(oldId)
    Q_UNUSED(newId)
}
