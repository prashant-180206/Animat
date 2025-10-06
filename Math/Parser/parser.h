#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include "qqmlintegration.h"
#include <QVariantMap>
#include <QJsonObject>
#include <memory>
#include "trackermanager.h"
#include "CommandFactory.h"

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
    TrackerManager *trackerManager() const { return m_trackerManager; }

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

    // Helper methods
    QStringList parseScriptToCommands(const QString &script) const;
};

#endif // PARSER_H
