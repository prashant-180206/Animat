#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include "ClickableMobject.h"
#include "ptvaluetracker.h"
#include "valuetracker.h"
#include <QObject>

class TrackerManager:public QObject
{

    struct TrackerData{
        QString name;
        QString value;
    };

    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVector<TrackerData> Trackerinfo READ Trackerinfo WRITE setTrackerinfo NOTIFY TrackerinfoChanged FINAL)
public:

    TrackerManager(QObject* parent =nullptr):QObject(parent) {}



    QVector<TrackerData> Trackerinfo(){
        return m_trackernames;
    }

    void setTrackerinfo(const QVector<TrackerData> &val)
    {
        m_trackernames=val;

        emit TrackerinfoChanged();
    }
    Q_INVOKABLE QString getTrackerInfoString() const{
        QString info;
        for (const auto &tracker : m_trackernames) {
            info += tracker.name + ": " + tracker.value + "\n";
        }
        return info.trimmed();
    };


protected:

    void addTracker(const QString &str,qreal val);;

    void addPtTracker(const QString &str,QPointF val);;

    void connectTracker(const QString &valname, ClickableMobject* mobj, const QString &property);

    void connectPtTracker(const QString &valname, ClickableMobject* mobj, const QString &property);

    Q_INVOKABLE void parse(QString code);

signals:
    void TrackerinfoChanged();


private:
    QMap<QString,ValueTracker*> m_vals;
    QMap<QString,PtValueTracker*> m_ptvals;

    QVector<TrackerData> m_trackernames;
};

#endif // TRACKERMANAGER_H
