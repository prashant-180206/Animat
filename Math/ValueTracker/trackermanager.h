#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H

#include "pttrackerdata.h"
#include "PtValueTracker.h"
#include "trackerdata.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <qquickitem.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class Scene;
class ValueTracker;

Q_DECLARE_METATYPE(QList<TrackerData *>)
Q_DECLARE_METATYPE(QList<PtTrackerData *>)

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
    Q_INVOKABLE ValueTracker *getValueTracker(const QString &name) const;
    Q_INVOKABLE PtValueTracker *getPtValueTracker(const QString &name) const;
    bool hasValueTracker(const QString &name) const;
    bool hasPointTracker(const QString &name) const;

    // Create new ValueTracker instances
    ValueTracker *createValueTracker(const QString &name, qreal initialValue = 0.0);
    PtValueTracker *createValueTracker(const QString &name, QPointF initialValue = {0, 0});

    // Get all tracker names
    Q_INVOKABLE QStringList getTrackerNames() const;
    Q_INVOKABLE QStringList getPointTrackerNames() const;

    // Get TrackerData properties
    Q_INVOKABLE qreal getTrackerMin(const QString &name) const;
    Q_INVOKABLE qreal getTrackerMax(const QString &name) const;
    Q_INVOKABLE QPointF getPointTrackerMin(const QString &name) const;
    Q_INVOKABLE QPointF getPointTrackerMax(const QString &name) const;
    Q_INVOKABLE bool hasSliderConfiguration(const QString &name) const;
    Q_INVOKABLE bool hasPointSliderConfiguration(const QString &name) const;

    // Scene access
    Scene *scene() const { return canvas; }

    QList<TrackerData *> activeTrackers() const;
    void setActiveTrackers(const QList<TrackerData *> &newActiveTrackers);

    QList<PtTrackerData *> activePtTrackers() const;
    void setActivePtTrackers(const QList<PtTrackerData *> &newActivePtTrackers);

    Q_INVOKABLE void addSlider(const QString &s, qreal maxval, qreal minval, qreal aprtime);

    Q_INVOKABLE void addSlider(const QString &s, QPointF maxval, QPointF minval, qreal aprtime);

    Q_INVOKABLE void Continue();

    struct TrackerManagerData
    {
        struct ValueTrackerData
        {
            QString name = "";
            qreal value = 0.0;
            qreal min = 0.0;
            qreal max = 0.0;
            qreal appeartime = 2000;
        };
        struct PtValueTrackerData
        {
            QString name = "";
            QPointF value = {0, 0};
            QPointF min = {0, 0};
            QPointF max = {0, 0};
            qreal appeartime = 2000;
        };

        QList<ValueTrackerData> Trackers;
        QList<PtValueTrackerData> PtTrackers;

        QJsonDocument toJson() const
        {
            QJsonObject obj;
            QJsonArray trackerArray;
            for (const auto &tracker : Trackers)
            {
                QJsonObject tObj;
                tObj["name"] = tracker.name;
                tObj["value"] = tracker.value;
                tObj["min"] = tracker.min;
                tObj["max"] = tracker.max;
                tObj["appeartime"] = tracker.appeartime;
                trackerArray.append(tObj);
            }
            obj["Trackers"] = trackerArray;

            QJsonArray ptTrackerArray;
            for (const auto &tracker : PtTrackers)
            {
                QJsonObject tObj;
                tObj["name"] = tracker.name;
                tObj["value"] = QJsonObject{{"x", tracker.value.x()}, {"y", tracker.value.y()}};
                tObj["min"] = QJsonObject{{"x", tracker.min.x()}, {"y", tracker.min.y()}};
                tObj["max"] = QJsonObject{{"x", tracker.max.x()}, {"y", tracker.max.y()}};
                tObj["appeartime"] = tracker.appeartime;
                ptTrackerArray.append(tObj);
            }
            obj["PtTrackers"] = ptTrackerArray;

            return QJsonDocument(obj);
        };

        static TrackerManagerData fromJSON(const QJsonObject &o)
        {
            TrackerManagerData data;
            if (o.contains("Trackers") && o["Trackers"].isArray())
            {
                QJsonArray trackerArray = o["Trackers"].toArray();
                for (const auto &t : trackerArray)
                {
                    if (t.isObject())
                    {
                        QJsonObject tObj = t.toObject();
                        ValueTrackerData vt;
                        vt.name = tObj["name"].toString();
                        vt.value = tObj["value"].toDouble();
                        vt.min = tObj["min"].toDouble();
                        vt.max = tObj["max"].toDouble();
                        vt.appeartime = tObj["appeartime"].toDouble();
                        data.Trackers.append(vt);
                    }
                }
            }

            if (o.contains("PtTrackers") && o["PtTrackers"].isArray())
            {
                QJsonArray ptTrackerArray = o["PtTrackers"].toArray();
                for (const auto &t : ptTrackerArray)
                {
                    if (t.isObject())
                    {
                        QJsonObject tObj = t.toObject();
                        PtValueTrackerData pvt;
                        pvt.name = tObj["name"].toString();
                        if (tObj["value"].isObject())
                        {
                            QJsonObject valObj = tObj["value"].toObject();
                            pvt.value = QPointF(valObj["x"].toDouble(), valObj["y"].toDouble());
                        }
                        if (tObj["min"].isObject())
                        {
                            QJsonObject minObj = tObj["min"].toObject();
                            pvt.min = QPointF(minObj["x"].toDouble(), minObj["y"].toDouble());
                        }
                        if (tObj["max"].isObject())
                        {
                            QJsonObject maxObj = tObj["max"].toObject();
                            pvt.max = QPointF(maxObj["x"].toDouble(), maxObj["y"].toDouble());
                        }
                        pvt.appeartime = tObj["appeartime"].toDouble();
                        data.PtTrackers.append(pvt);
                    }
                }
            }

            return data;
        };
    };

    TrackerManagerData getData() const
    {
        TrackerManagerData data;

        for (auto t : getTrackerNames())
        {
            TrackerManagerData::ValueTrackerData vt;
            vt.name = t;
            vt.value = m_valueTrackers[t]->tracker()->value();
            vt.min = m_valueTrackers[t]->min();
            vt.max = m_valueTrackers[t]->max();
            vt.appeartime = m_valueTrackers[t]->appeartime();
            data.Trackers.append(vt);
        }

        for (auto t : getPointTrackerNames())
        {
            TrackerManagerData::PtValueTrackerData pvt;
            pvt.name = t;
            pvt.value = m_ptvalueTrackers[t]->tracker()->value();
            pvt.min = m_ptvalueTrackers[t]->min();
            pvt.max = m_ptvalueTrackers[t]->max();
            pvt.appeartime = m_ptvalueTrackers[t]->appeartime();
            data.PtTrackers.append(pvt);
        }

        return data;
    };

    void setFromJSON(QJsonObject &o)
    {
        TrackerManagerData data = TrackerManagerData::fromJSON(o);

        // Clear existing trackers
        // clearAllTrackers();

        // Add value trackers
        for (const auto &vt : data.Trackers)
        {
            ValueTracker *tracker = createValueTracker(vt.name, vt.value);
            if (tracker)
            {
                TrackerData *td = m_valueTrackers.value(vt.name, nullptr);
                if (td)
                {
                    td->setMin(vt.min);
                    td->setMax(vt.max);
                    td->setAppeartime(vt.appeartime);
                    emit sliderConfigurationChanged(vt.name);
                }
            }
        }
        // Add point trackers
        for (const auto &pvt : data.PtTrackers)
        {
            PtValueTracker *tracker = createValueTracker(pvt.name, pvt.value);
            if (tracker)
            {
                PtTrackerData *td = m_ptvalueTrackers.value(pvt.name, nullptr);
                if (td)
                {
                    td->setMin(pvt.min);
                    td->setMax(pvt.max);
                    td->setAppeartime(pvt.appeartime);
                    emit sliderConfigurationChanged(pvt.name);
                }
            }
        }
    }

signals:
    void trackerAdded(const QString &name);
    void trackerRemoved(const QString &name);
    void sliderConfigurationChanged(const QString &name);

    void activeTrackersChanged();
    void activePtTrackersChanged();

public slots:
    void clearAllTrackers();
    void changeActiveTrackers(qreal t);

private:
    Scene *canvas;
    QHash<QString, TrackerData *> m_valueTrackers;
    QHash<QString, PtTrackerData *> m_ptvalueTrackers;

    QList<TrackerData *> m_activeTrackers;
    QList<PtTrackerData *> m_activePtTrackers;

    QSet<TrackerData *> m_completedTrackers;
    QSet<PtTrackerData *> m_completedPtTrackers;

    Q_PROPERTY(QList<TrackerData *> activeTrackers READ activeTrackers WRITE setActiveTrackers NOTIFY activeTrackersChanged FINAL)
    Q_PROPERTY(QList<PtTrackerData *> activePtTrackers READ activePtTrackers WRITE setActivePtTrackers NOTIFY activePtTrackersChanged FINAL)
};

#endif // TRACKERMANAGER_H
