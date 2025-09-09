#include "trackermanager.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>



void TrackerManager::addTracker(const QString &str,qreal val=0){
    if(m_vals.contains(str)){
        m_vals[str]->deleteLater();
    }

    m_trackernames.append(TrackerData{
        str,QString::number(val,'f',2)
    });

    m_vals[str]= new ValueTracker();
}

void TrackerManager::addPtTracker(const QString &str,QPointF val={0,0}){
    if(m_ptvals.contains(str)){
        m_ptvals[str]->deleteLater();
    }

    m_trackernames.append(TrackerData{
        str,QString("(%1, %2)")
        .arg(val.x(), 0, 'f', 2)
            .arg(val.y(), 0, 'f', 2)
    });

    m_ptvals[str]= new PtValueTracker();
}

void TrackerManager::connectTracker(const QString &valname, ClickableMobject *mobj, const QString &property) {
    ValueTracker* tracker = m_vals[valname];
    if (!tracker){
        qInfo()<<"INVALID VALNAME";
        return;
    }

    QObject::connect(tracker,&ValueTracker::valueChanged, mobj, [mobj,property](qreal v){
        if(property =="height"){
            auto pt = mobj->getProperties()->size();
            mobj->getProperties()->setSize({v,pt.y()});

        }else if(property =="width"){
            auto pt = mobj->getProperties()->size();
            mobj->getProperties()->setSize({pt.x(),v});

        }else if (property =="radius"){
            mobj->getProperties()->setRadius(v);

        }else{
            qInfo()<<"INVALID PROPERTY";
        }
    });
}

void TrackerManager::connectPtTracker(const QString &valname, ClickableMobject *mobj, const QString &property) {
    PtValueTracker* tracker = m_ptvals[valname];
    if (!tracker){
        qInfo()<<"INVALID VALNAME";
        return;
    }

    QObject::connect(tracker,&PtValueTracker::valueChanged, mobj, [mobj,property](QPointF v){
        if(property =="size"){
            mobj->getProperties()->setSize(v);

        }else if(property =="pos"){

            mobj->getProperties()->setSize(v);

        }else if (property =="linestart"){
            mobj->getProperties()->setLineStart(v);

        }else if (property =="lineend"){
            mobj->getProperties()->setLineEnd(v);

        }else if (property =="trange"){
            mobj->getProperties()->setTRange(v);

        }else{
            qInfo()<<"INVALID PROPERTY";
        }
    });
}

void TrackerManager::parse(QString code) {
    QRegularExpression valRegex(R"(^\s*val\s+(\w+)\s*=\s*([-+]?\d*\.?\d+)\s*$)");
    QRegularExpression pvalRegex(R"(^\s*pval\s+(\w+)\s*=\s*\(\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\)\s*$)");
    QRegularExpression connectRegex(R"(^\s*connect\s*\(\s*(\w+)\s*,\s*mobj\.(\w+)\s*\)\s*$)");

    QStringList lines = code.split('\n', Qt::SkipEmptyParts);
    qInfo() << "PARSE CALLED with code containing" << lines.size() << "lines";

    for (const QString &line : std::as_const(lines)) {
        QString trimmedLine = line.trimmed();
        qInfo() << "Parsing line:" << trimmedLine;
        if (trimmedLine.isEmpty()) {
            qInfo() << "Skipped empty line";
            continue;
        }

        QRegularExpressionMatch match;

        // Match val pattern: val valname = 0
        match = valRegex.match(trimmedLine);
        if (match.hasMatch()) {
            QString valname = match.captured(1);
            bool ok = false;
            qreal value = match.captured(2).toDouble(&ok);
            if (ok) {
                qInfo() << "Matched val:" << valname << "=" << value;
                addTracker(valname, value);
            } else {
                qWarning() << "Failed to convert val number:" << match.captured(2);
            }
            continue;
        }

        // Match pval pattern: pval valname = (0,0)
        match = pvalRegex.match(trimmedLine);
        if (match.hasMatch()) {
            QString valname = match.captured(1);
            bool okX = false, okY = false;
            qreal x = match.captured(2).toDouble(&okX);
            qreal y = match.captured(3).toDouble(&okY);
            if (okX && okY) {
                qInfo() << "Matched pval:" << valname << "=" << "(" << x << "," << y << ")";
                addPtTracker(valname, QPointF(x, y));
            } else {
                qWarning() << "Failed to convert pval coordinates:" << match.captured(2) << "," << match.captured(3);
            }
            continue;
        }

        // Match connect pattern: connect(valname, mobj.property)
        match = connectRegex.match(trimmedLine);
        if (match.hasMatch()) {
            QString valname = match.captured(1);
            QString property = match.captured(2);
            qInfo() << "Connect statement detected for valname:" << valname << "property:" << property;
            // Connection code here if mobj accessible
            continue;
        }

        // If line doesn't match any pattern, optionally log
        qWarning() << "Unrecognized line in parse:" << trimmedLine;
    }
}



