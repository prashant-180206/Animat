#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include "ptvaluetracker.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <qquickitem.h>

class Scene;
class ValueTracker;

class TrackerManager : public QQuickItem
{
    Q_OBJECT
public:
    explicit TrackerManager(Scene *c, QObject *parent = nullptr)
        : canvas(c), QQuickItem{}
    {
    }

    // ValueTracker management
    void addValueTracker(const QString &name, ValueTracker *tracker);
    void addValueTracker(const QString &name, PtValueTracker *tracker);
    void removeValueTracker(const QString &name);
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

signals:
    void trackerAdded(const QString &name);
    void trackerRemoved(const QString &name);

public slots:
    void clearAllTrackers();

private:
    Scene *canvas;
    QHash<QString, ValueTracker *> m_valueTrackers;
    QHash<QString, PtValueTracker *> m_ptvalueTrackers;
};

#endif // TRACKERMANAGER_H
