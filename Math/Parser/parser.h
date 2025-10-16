#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include "qqmlintegration.h"
#include <QVariantMap>
#include <QJsonObject>
#include "../ValueTracker/trackermanager.h"
#include "CommandFactory.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class Scene;

class Parser : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Parser(Scene *scene, QObject *parent = nullptr);

    // Command parsing functionality
    Q_INVOKABLE bool parseCommand(const QString &input);
    Q_INVOKABLE bool executeScript(const QString &script);
    Q_INVOKABLE bool executeCommands(const QStringList &commands);

    // Access to managers
    Q_INVOKABLE TrackerManager *trackerManager() const { return m_trackerManager; }

    // Metadata functions for value trackers
    Q_INVOKABLE QVariantMap getAllTrackerValues() const;
    Q_INVOKABLE QVariantMap getValueTrackerMetadata() const;
    Q_INVOKABLE QVariantMap getPointTrackerMetadata() const;
    Q_INVOKABLE QJsonObject getTrackerMetadataAsJson() const;

    // Individual tracker queries
    Q_INVOKABLE qreal getTrackerValue(const QString &name) const;
    Q_INVOKABLE QPointF getPointTrackerValue(const QString &name) const;
    Q_INVOKABLE bool hasTracker(const QString &name) const;
    Q_INVOKABLE QStringList getTrackerNames() const;
    Q_INVOKABLE QStringList getPointTrackerNames() const;

    // Direct tracker updates (bypass command system)
    Q_INVOKABLE bool setTrackerValue(const QString &name, qreal value);
    Q_INVOKABLE bool setPointTrackerValue(const QString &name, const QPointF &point);
    Q_INVOKABLE bool setPointTrackerValue(const QString &name, qreal x, qreal y);
    Q_INVOKABLE bool deleteTracker(const QString &name);
    Q_INVOKABLE bool deletePointTracker(const QString &name);

    Q_INVOKABLE void setInitCommand(const QString &newInitCommand);

    struct ParserData
    {
        QString instructions;
        TrackerManager::TrackerManagerData trackerData;

        QJsonDocument toJson() const
        {
            QJsonObject obj;
            obj["instructions"] = instructions;
            obj["trackerData"] = trackerData.toJson().object();
            return QJsonDocument(obj);
        }

        static ParserData fromJSON(const QJsonObject &o)
        {
            ParserData data;
            if (o.contains("instructions") && o["instructions"].isString())
                data.instructions = o["instructions"].toString();
            if (o.contains("trackerData") && o["trackerData"].isObject())
                data.trackerData = TrackerManager::TrackerManagerData::fromJSON(o["trackerData"].toObject());
            return data;
        }
    };

    ParserData getData() const
    {
        ParserData data;
        data.instructions = m_initCommand;
        data.trackerData = m_trackerManager->getData();
        return data;
    };
    void setFromJSON(const QJsonObject &obj)
    {
        ParserData data = ParserData::fromJSON(obj);
        m_trackerManager->clearAllTrackers();
        m_initCommand = data.instructions;
        if (!m_initCommand.isEmpty())
            executeScript(m_initCommand);

        auto doc = obj["trackerData"].toObject();

        if (doc.isEmpty())
            return;
        m_trackerManager->setFromJSON(doc);
    };

signals:
    void commandExecuted(const QString &commandName, const QString &input);
    void commandFailed(const QString &input, const QString &error);
    void trackerCreated(const QString &name);
    void trackerRemoved(const QString &name);
    void scriptExecuted(bool success, const QString &message);

private:
    Scene *m_scene;
    TrackerManager *m_trackerManager;
    CommandFactory m_commandFactory;

    QString m_initCommand = "";

    // Helper methods
    QStringList parseScriptToCommands(const QString &script) const;
};

#endif // PARSER_H
