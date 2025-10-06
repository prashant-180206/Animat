#include "parser.h"
#include "../Scene.h"
#include "CommandFactory.h"
#include <QDebug>
#include <QJsonDocument>
#include <QRegularExpression>

Parser::Parser(Scene *scene, QObject *parent)
    : QObject(parent), m_scene(scene), m_commandFactory()
{
    // Initialize TrackerManager
    m_trackerManager = new TrackerManager(scene, this);

    // Connect TrackerManager signals
    connect(m_trackerManager, &TrackerManager::trackerAdded,
            this, &Parser::trackerCreated);
    connect(m_trackerManager, &TrackerManager::trackerRemoved,
            this, &Parser::trackerRemoved);
}

bool Parser::parseCommand(const QString &input)
{
    if (input.trimmed().isEmpty())
    {
        emit commandFailed(input, "Empty command");
        return false;
    }

    auto command = m_commandFactory.createCommand(input);

    if (command)
    {
        try
        {
            command->execute(m_scene, m_trackerManager);
            emit commandExecuted("Command", input);
            return true;
        }
        catch (const std::exception &e)
        {
            emit commandFailed(input, QString("Execution error: %1").arg(e.what()));
            return false;
        }
    }
    else
    {
        emit commandFailed(input, "No matching command found");
        return false;
    }
}

bool Parser::executeScript(const QString &script)
{
    QStringList commands = parseScriptToCommands(script);
    return executeCommands(commands);
}

bool Parser::executeCommands(const QStringList &commands)
{
    bool allSuccess = true;
    QString errorMessage;

    for (const QString &command : commands)
    {
        if (!parseCommand(command))
        {
            allSuccess = false;
            errorMessage += QString("Failed: %1\n").arg(command);
        }
    }

    emit scriptExecuted(allSuccess, allSuccess ? "Script executed successfully" : errorMessage);
    return allSuccess;
}

QStringList Parser::parseScriptToCommands(const QString &script) const
{
    QStringList commands;

    // Split by semicolons and clean up each command
    QStringList rawCommands = script.split(';', Qt::SkipEmptyParts);

    for (const QString &rawCommand : rawCommands)
    {
        QString cleanCommand = rawCommand.trimmed();

        // Skip empty commands and comments
        if (cleanCommand.isEmpty() || cleanCommand.startsWith("//"))
        {
            continue;
        }

        // Add semicolon back if not present (needed for regex matching)
        if (!cleanCommand.endsWith(';'))
        {
            cleanCommand += ';';
        }

        commands.append(cleanCommand);
    }

    return commands;
}

QVariantMap Parser::getAllTrackerValues() const
{
    QVariantMap result;

    // Add value trackers
    QStringList valueTrackerNames = m_trackerManager->getTrackerNames();
    for (const QString &name : valueTrackerNames)
    {
        ValueTracker *tracker = m_trackerManager->getValueTracker(name);
        if (tracker)
        {
            result[name] = tracker->value();
        }
    }

    // Add point trackers
    QStringList pointTrackerNames = getPointTrackerNames();
    for (const QString &name : pointTrackerNames)
    {
        PtValueTracker *tracker = m_trackerManager->getPtValueTracker(name);
        if (tracker)
        {
            QVariantMap pointData;
            pointData["x"] = tracker->value().x();
            pointData["y"] = tracker->value().y();
            result[name] = pointData;
        }
    }

    return result;
}

QVariantMap Parser::getValueTrackerMetadata() const
{
    QVariantMap metadata;
    QStringList trackerNames = m_trackerManager->getTrackerNames();

    for (const QString &name : trackerNames)
    {
        ValueTracker *tracker = m_trackerManager->getValueTracker(name);
        if (tracker)
        {
            QVariantMap trackerInfo;
            trackerInfo["name"] = name;
            trackerInfo["value"] = tracker->value();
            trackerInfo["type"] = "qreal";
            trackerInfo["exists"] = true;
            metadata[name] = trackerInfo;
        }
    }

    return metadata;
}

QVariantMap Parser::getPointTrackerMetadata() const
{
    QVariantMap metadata;
    QStringList pointTrackerNames = getPointTrackerNames();

    for (const QString &name : pointTrackerNames)
    {
        PtValueTracker *tracker = m_trackerManager->getPtValueTracker(name);
        if (tracker)
        {
            QVariantMap trackerInfo;
            trackerInfo["name"] = name;
            QVariantMap pointValue;
            pointValue["x"] = tracker->value().x();
            pointValue["y"] = tracker->value().y();
            trackerInfo["value"] = pointValue;
            trackerInfo["type"] = "QPointF";
            trackerInfo["exists"] = true;
            metadata[name] = trackerInfo;
        }
    }

    return metadata;
}

QJsonObject Parser::getTrackerMetadataAsJson() const
{
    QVariantMap allMetadata;
    allMetadata["valueTrackers"] = getValueTrackerMetadata();
    allMetadata["pointTrackers"] = getPointTrackerMetadata();
    allMetadata["totalCount"] = getTrackerNames().size() + getPointTrackerNames().size();

    QJsonDocument doc = QJsonDocument::fromVariant(allMetadata);
    return doc.object();
}

qreal Parser::getTrackerValue(const QString &name) const
{
    ValueTracker *tracker = m_trackerManager->getValueTracker(name);
    return tracker ? tracker->value() : 0.0;
}

QPointF Parser::getPointTrackerValue(const QString &name) const
{
    PtValueTracker *tracker = m_trackerManager->getPtValueTracker(name);
    return tracker ? tracker->value() : QPointF(0, 0);
}

bool Parser::hasTracker(const QString &name) const
{
    return m_trackerManager->hasValueTracker(name) || m_trackerManager->hasPointTracker(name);
}

QStringList Parser::getTrackerNames() const
{
    return m_trackerManager->getTrackerNames();
}

QStringList Parser::getPointTrackerNames() const
{
    return m_trackerManager->getPointTrackerNames();
}
