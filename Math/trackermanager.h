#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H
#include "ClickableMobject.h"
#include "ptvaluetracker.h"
#include "valuetracker.h"
#include <QObject>
#include <QRegularExpression>
#include <QVector>
#include <functional>

class TrackerManager : public QObject
{
    struct TrackerData {
        QString name;
        QString value;
    };
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVector<TrackerData> Trackerinfo READ Trackerinfo WRITE setTrackerinfo NOTIFY TrackerinfoChanged FINAL)
public:
    TrackerManager(QObject* parent = nullptr);

    QVector<TrackerData> Trackerinfo();
    void setTrackerinfo(const QVector<TrackerData>& val);

    void addTracker(const QString& str, qreal val);

    void addPtTracker(const QString& str, QPointF val);

    void connectTracker(const QString& valname, ClickableMobject* mobj, const QString& property);

    void connectPtTracker(const QString& valname, ClickableMobject* mobj, const QString& property);

    Q_INVOKABLE void parse(QString code);

    using ValFunc = std::function<void(ClickableMobject*, qreal)>;
    using PtValFunc = std::function<void(ClickableMobject*, QPointF)>;

    inline static const QMap<QString, ValFunc> s_valueTrackerFuncs = {
        { "height", [](ClickableMobject* mobj, qreal v) {
             auto pt = mobj->getProperties()->size();
             mobj->getProperties()->setSize({ v, pt.y() });
         }},
        { "width", [](ClickableMobject* mobj, qreal v) {
             auto pt = mobj->getProperties()->size();
             mobj->getProperties()->setSize({ pt.x(), v });
         }},
        { "radius", [](ClickableMobject* mobj, qreal v) {
             mobj->getProperties()->setRadius(v);
         }},
        { "opacity", [](ClickableMobject* mobj, qreal v) {
             mobj->getProperties()->setOpacity(v);
         }}
    };

    inline static const QMap<QString, PtValFunc> s_ptValueTrackerFuncs = {
        { "size", [](ClickableMobject* mobj, QPointF v) {
             mobj->getProperties()->setSize(v);
         }},
        { "pos", [](ClickableMobject* mobj, QPointF v) {
             mobj->getProperties()->setPos(v);
         }},
        { "linestart", [](ClickableMobject* mobj, QPointF v) {
             mobj->getProperties()->setLineStart(v);
         }},
        { "lineend", [](ClickableMobject* mobj, QPointF v) {
             mobj->getProperties()->setLineEnd(v);
         }},
        { "trange", [](ClickableMobject* mobj, QPointF v) {
             mobj->getProperties()->setTRange(v);
         }}
    };


signals:
    void TrackerinfoChanged();

private:
    QMap<QString, ValueTracker*> m_vals;
    QMap<QString, PtValueTracker*> m_ptvals;
    QVector<TrackerData> m_trackernames;

    // Regex patterns as static members
    static const QRegularExpression s_valRegex;
    static const QRegularExpression s_pvalRegex;
    static const QRegularExpression s_connectRegex;

    // Static function maps for property -> lambda for ValueTracker and PtValueTracker



    // Non-static parse command list member (changed from QMap to QVector)
    QVector<std::pair<QRegularExpression, std::function<void(const QRegularExpressionMatch&)>>> m_parseCommandList;
};
#endif // TRACKERMANAGER_H
