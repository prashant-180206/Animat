#ifndef CIRCLEPROPERTIES_H
#define CIRCLEPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qjsondocument.h>
#include <QJsonObject>

class CircleProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(int segments READ segments WRITE setSegments NOTIFY segmentsChanged FINAL)

public:
    explicit CircleProperties(QObject *parent = nullptr);
    qreal radius() const;
    int segments() { return m_segments; }

    struct CirclePropData
    {
        qreal radius = 0;
        int segments = 30;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["radius"] = radius;
            o["segments"] = segments;
            return QJsonDocument(o);
        }
        static CirclePropData fromJSON(const QJsonObject &o)
        {
            CirclePropData d;
            d.radius = o["radius"].toDouble();
            d.segments = o["segments"].toInt();
            return d;
        }
    };

    CirclePropData getData() const
    {
        CirclePropData d;
        d.radius = m_radius;
        d.segments = m_segments;
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        CirclePropData d = CirclePropData::fromJSON(o);
        setRadius(d.radius);
        setSegments(d.segments);
    }

public slots:
    void setRadius(qreal radius);
    void setSegments(int segments);
signals:
    void radiusChanged();
    void segmentsChanged();

private:
    qreal m_radius = 0;
    int m_segments = 30;
};

#endif // CIRCLEPROPERTIES_H
