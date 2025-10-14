#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include "pttrackerdata.h"
#include "PtValueTracker.h"
#include "trackerdata.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <qquickitem.h>

class Scene;
class ValueTracker;

Q_DECLARE_METATYPE(QList<TrackerData*>)
Q_DECLARE_METATYPE(QList<PtTrackerData*>)

class TrackerManager : public QQuickItem
{
    Q_OBJECT
public:
    explicit TrackerManager(Scene *c, QObject *parent = nullptr);

    // ValueTracker management
    void addValueTracker(const QString &name, TrackerData *tracker);
    void addValueTracker(const QString &name, PtTrackerData *tracker);
    void removeValueTracker(const QString &name);
    void removePtValueTracker(const QString &name);
    ValueTracker *getValueTracker(const QString &name) const;
    PtValueTracker *getPtValueTracker(const QString &name) const;
    bool hasValueTracker(const QString &name) const;
    bool hasPointTracker(const QString &name) const;

    // Create new ValueTracker instances
    ValueTracker *createValueTracker(const QString &name, qreal initialValue = 0.0);
    PtValueTracker *createValueTracker(const QString &name, QPointF initialValue = {0, 0});

    // Get all tracker names
    QStringList getTrackerNames() const;
    QStringList getPointTrackerNames() const;

    // Scene access
    Scene *scene() const { return canvas; }

    QList<TrackerData*> activeTrackers() const;
    void setActiveTrackers(const QList<TrackerData*> &newActiveTrackers);

    QList<PtTrackerData*> activePtTrackers() const;
    void setActivePtTrackers(const QList<PtTrackerData*> &newActivePtTrackers);

signals:
    void trackerAdded(const QString &name);
    void trackerRemoved(const QString &name);

    void activeTrackersChanged();
    void activePtTrackersChanged();

public slots:
    void clearAllTrackers();
    void changeActiveTrackers(qreal t);
    void onContinue();

private:
    Scene *canvas;
    QHash<QString, TrackerData *> m_valueTrackers;
    QHash<QString, PtTrackerData *> m_ptvalueTrackers;

    QList<TrackerData*> m_activeTrackers;
    QList<PtTrackerData*> m_activePtTrackers;

    QSet<TrackerData *> m_completedTrackers;
    QSet<PtTrackerData * > m_completedPtTrackers;

    Q_PROPERTY(QList<TrackerData*> activeTrackers READ activeTrackers WRITE setActiveTrackers NOTIFY activeTrackersChanged FINAL)
    Q_PROPERTY(QList<PtTrackerData*> activePtTrackers READ activePtTrackers WRITE setActivePtTrackers NOTIFY activePtTrackersChanged FINAL)
};

#endif // TRACKERMANAGER_H
