#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QPointF>

#include "trackercommand.h"
#include "animationscriptparser.h"
#include "dependencygraph.h"
#include "parser.h"
#include "valuetracker.h"
#include "ptvaluetracker.h"

class Scene;
class ClickableMobject;

class TrackerManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    struct TrackerInfo
    {
        QString name;
        QString displayValue;
        QString expression;
        bool isExpression;
        bool isDynamic;
    };

    Q_PROPERTY(QVector<TrackerInfo> trackerInfo READ getTrackerInfo NOTIFY trackerInfoChanged)

public:
    explicit TrackerManager(Scene *canvas, QObject *parent = nullptr);
    ~TrackerManager();

    // Main interface
    Q_INVOKABLE void parseScript(const QString &script);
    Q_INVOKABLE void setMasterVariable(const QString &name, double value);
    Q_INVOKABLE void updateTracker(const QString &name, double value);

    // Getters
    QVector<TrackerInfo> getTrackerInfo() const;
    ValueTracker *getValueTracker(const QString &name) const;
    PtValueTracker *getPointTracker(const QString &name) const;
    Q_INVOKABLE double getTrackerValue(const QString &name) const;
    Q_INVOKABLE QPointF getTrackerPoint(const QString &name) const;
    Q_INVOKABLE QStringList getAllValueTrackerNames() const;
    Q_INVOKABLE QStringList getAllPointTrackerNames() const;

    // Connection management
    void connectTracker(const QString &trackerName, ClickableMobject *object, const QString &property);
    void disconnectTracker(const QString &trackerName, ClickableMobject *object);

    // Control
    void clearAllTrackers();
    void removeTracker(const QString &name);

signals:
    void trackerInfoChanged();
    void trackerValueChanged(const QString &name, double value);
    void trackerPointChanged(const QString &name, QPointF point);
    void trackersListChanged(); // Signal when trackers are added/removed

private slots:
    void onTrackerValueChanged(double value);
    void onTrackerPointChanged(QPointF point);

private:
    // Core components
    Scene *m_canvas;
    Parser *m_parser;
    AnimationScriptParser *m_scriptParser;
    DependencyGraph *m_dependencyGraph;

    // Tracker storage
    QHash<QString, ValueTracker *> m_valueTrackers;
    QHash<QString, PtValueTracker *> m_pointTrackers;
    QVector<TrackerInfo> m_trackerInfo;

    // Dynamic tracker expressions (for updates)
    QHash<QString, QString> m_dynamicExpressions;
    QHash<QString, QPair<QString, QString>> m_dynamicPointExpressions;

    // Regular expression trackers (time-based, etc.)
    QHash<QString, QString> m_regularExpressions;
    QHash<QString, QPair<QString, QString>> m_regularPointExpressions;

    // Connection tracking
    struct Connection
    {
        ClickableMobject *object;
        QString property;
    };
    QHash<QString, QVector<Connection>> m_connections; // tracker name -> connections

    // Helper methods
    void executeCommand(const TrackerCommand &command);
    void createValueTracker(const TrackerCommand &command);
    void createPointTracker(const TrackerCommand &command);
    void createConnection(const TrackerCommand &command);

    void updateDynamicTrackers();
    void updateRegularExpressionTrackers();
    void updateSingleDynamicTracker(const QString &name);
    void updateTrackerInfo();
    void updateConnectedObjects(const QString &trackerName);

    QHash<QString, double> getCurrentTrackerValues() const;

    TrackerInfo createTrackerInfo(const QString &name) const;
};

#endif // TRACKERMANAGER_H