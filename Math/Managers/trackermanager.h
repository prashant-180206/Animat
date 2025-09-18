#ifndef TRACKERMANAGER_H
#define TRACKERMANAGER_H
#include "ClickableMobject.h"
#include "ptvaluetracker.h"
#include "valuetracker.h"
#include <QObject>
#include <QRegularExpression>
#include <QVector>

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
    TrackerManager(Scene* canvas,QObject* parent = nullptr);

    QVector<TrackerData> Trackerinfo();;
    void setTrackerinfo(const QVector<TrackerData>& val);

    void addTracker(const QString& str, qreal val);

    void addPtTracker(const QString& str, QPointF val);

    void connectTracker(const QString& valname, ClickableMobject* mobj, const QString& property);

    void connectPtTracker(const QString& valname, ClickableMobject* mobj, const QString& property);

    Q_INVOKABLE void parse(QString code);

signals:
    void TrackerinfoChanged();

private:
    QHash<QString, ValueTracker*> m_vals;
    QHash<QString, PtValueTracker*> m_ptvals;
    QVector<TrackerData> m_trackernames;

    // Pattern 1: val valname = 2;
    QRegularExpression reVal{R"(val\s+(\w+)\s*=\s*(\d+);)"};
    // Pattern 2: pval valname = (1,1)
    QRegularExpression rePval{R"(pval\s+(\w+)\s*=\s*\(\s*([+-]?\d+)\s*,\s*([+-]?\d+)\s*\);)"};
    // Pattern 3: connect(valname, objname.propname)
    QRegularExpression reConnect{R"(connect\s*\(\s*(\w+)\s*,\s*(\w+)\.(\w+)\s*\);)"};

    Scene * m_canvas=nullptr;


};
#endif // TRACKERMANAGER_H
