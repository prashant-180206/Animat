#include "funcmap.h"
#include "trackermanager.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "Scene.h"

QVector<TrackerManager::TrackerData> TrackerManager::Trackerinfo(){return m_trackernames;}

TrackerManager::TrackerManager(Scene *canvas, QObject *parent) : QObject(parent)
{
}

void TrackerManager::setTrackerinfo(const QVector<TrackerData> &val)
{
    m_trackernames = val;
    emit TrackerinfoChanged();
}

void TrackerManager::addTracker(const QString &str, qreal val) {
    if (m_vals.contains(str)) {
        m_vals[str]->deleteLater();
    }
    m_trackernames.append(TrackerData{
        str, QString::number(val, 'f', 2)
    });
    m_vals[str] = new ValueTracker();
}

void TrackerManager::addPtTracker(const QString &str, QPointF val) {
    if (m_ptvals.contains(str)) {
        m_ptvals[str]->deleteLater();
    }
    m_trackernames.append(TrackerData{
        str, QString("(%1, %2)")
        .arg(val.x(), 0, 'f', 2)
            .arg(val.y(), 0, 'f', 2)
    });
    m_ptvals[str] = new PtValueTracker();
}

void TrackerManager::connectTracker(const QString &valname, ClickableMobject *mobj, const QString &property) {
    auto val = m_vals[valname];
    if(!val) return;

    connect (val,&ValueTracker::valueChanged,mobj,[this,property,mobj](qreal v){
        auto func = FuncMap::ConnectFunc[property];
        if(!func) return;
        func(mobj,v);
    });
}

void TrackerManager::connectPtTracker(const QString &valname, ClickableMobject *mobj, const QString &property) {
    auto val = m_ptvals[valname];
    if(!val) return;

    connect (val,&PtValueTracker::valueChanged,mobj,[this,property,mobj](QPointF v){
        auto func = FuncMap::PtConnectFunc[property];
        if(!func) return;
        func(mobj,v);
    });
}

void TrackerManager::parse(QString code) {

    auto lines = code.split("\n");
    for (const auto &line: std::as_const(lines)){

        QRegularExpressionMatch match;

        match = reVal.match(line);
        if(match.hasMatch()){
            QString name = match.captured(1);
            qreal val = match.captured(2).toDouble();
            addTracker(name,val);
            continue;
        }

        match = rePval.match(line);
        if(match.hasMatch()){
            QString name = match.captured(1);
            QPointF pt = {match.captured(2).toDouble(),match.captured(3).toDouble()};
            addPtTracker(name,pt);
            continue;
        }

        match = reConnect.match(line);
        if(match.hasMatch()){
            QString valname= match.captured(1);
            QString mobjname= match.captured(1);
            QString propname= match.captured(1);

            auto mobj = m_canvas->getMobject(mobjname);
            if (!mobj) continue;
            if(m_vals.contains(valname)){
                connectTracker(valname,mobj,propname);
            }else if(m_ptvals.contains(valname)){
                connectPtTracker(valname,mobj,propname);
            }else{
                continue;
            }
        }
    }
}








