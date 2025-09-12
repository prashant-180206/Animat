#include "trackermanager.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>


TrackerManager::TrackerManager(QObject *parent) : QObject(parent)
{
    // Initialize the parse command list (non-static)
    m_parseCommandList = {
        // val pattern: val valname = 0
        { QRegularExpression(R"(^\s*val\s+(\w+)\s*=\s*([-+]?\d*\.?\d+)\s*$)"),
            [this](const QRegularExpressionMatch& match) {
                QString valname = match.captured(1);
                bool ok = false;
                qreal value = match.captured(2).toDouble(&ok);
                if (ok) {
                    qInfo() << "Matched val:" << valname << "=" << value;
                    addTracker(valname, value);
                } else {
                    qWarning() << "Failed to convert val number:" << match.captured(2);
                }
            }
        },
        // pval pattern: pval valname = (0,0)
        { QRegularExpression(R"(^\s*pval\s+(\w+)\s*=\s*\(\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\)\s*$)"),
            [this](const QRegularExpressionMatch& match) {
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
            }
        },
        // connect pattern: connect(valname, mobj.property)
        { QRegularExpression(R"(^\s*connect\s*\(\s*(\w+)\s*,\s*mobj\.(\w+)\s*\)\s*$)"),
            [](const QRegularExpressionMatch& match) {
                QString valname = match.captured(1);
                QString property = match.captured(2);
                qInfo() << "Connect statement detected for valname:" << valname << "property:" << property;
                // No mobj context here to connect
            }
        }
    };
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
    ValueTracker* tracker = m_vals.value(valname, nullptr);
    auto funcIt = s_valueTrackerFuncs.constFind(property);
    if (!tracker || funcIt == s_valueTrackerFuncs.constEnd()) {
        qInfo() << "INVALID VALNAME or PROPERTY for ValueTracker:" << valname << property;
        return;
    }
    QObject::connect(tracker, &ValueTracker::valueChanged, mobj, [mobj, f = funcIt.value()](qreal v) {
        f(mobj, v);
    });
}

void TrackerManager::connectPtTracker(const QString &valname, ClickableMobject *mobj, const QString &property) {
    PtValueTracker* tracker = m_ptvals.value(valname, nullptr);
    auto funcIt = s_ptValueTrackerFuncs.constFind(property);
    if (!tracker || funcIt == s_ptValueTrackerFuncs.constEnd()) {
        qInfo() << "INVALID VALNAME or PROPERTY for PtValueTracker:" << valname << property;
        return;
    }
    QObject::connect(tracker, &PtValueTracker::valueChanged, mobj, [mobj, f = funcIt.value()](QPointF v) {
        f(mobj, v);
    });
}

void TrackerManager::parse(QString code) {
    QStringList lines = code.split('\n', Qt::SkipEmptyParts);
    qInfo() << "PARSE CALLED with code containing" << lines.size() << "lines";
    for (const QString& line : std::as_const(lines)) {
        QString trimmedLine = line.trimmed();
        qInfo() << "Parsing line:" << trimmedLine;
        if (trimmedLine.isEmpty()) {
            qInfo() << "Skipped empty line";
            continue;
        }
        bool matched = false;
        for (const auto& pair : std::as_const(m_parseCommandList)) {
            QRegularExpressionMatch match = pair.first.match(trimmedLine);
            if (match.hasMatch()) {
                matched = true;
                pair.second(match);
                break;
            }
        }
        if (!matched) {
            qWarning() << "Unrecognized line in parse:" << trimmedLine;
        }
    }
}

QVector<TrackerManager::TrackerData> TrackerManager::Trackerinfo() {
    return m_trackernames;
}
