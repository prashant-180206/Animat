#ifndef GEOMETRICPROPERTIES_H
#define GEOMETRICPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qqmlintegration.h>
#include <QPointer>
#include "Geometric/planeproperties.h"
#include "Geometric/angleproperties.h"

class GeometricProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(PlaneProperties *plane READ plane WRITE setPlane NOTIFY planeChanged)
    Q_PROPERTY(AngleProperties *angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
    explicit GeometricProperties(QObject *parent = nullptr);

    PlaneProperties *plane() const { return m_plane.data(); }
    void setPlane(PlaneProperties *plane);

    AngleProperties *angle() const { return m_angle.data(); }
    void setAngle(AngleProperties *angle);

    struct GeometricPropData
    {
        PlaneProperties::PlanePropData plane;
        AngleProperties::AnglePropData angle;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["plane"] = plane.toJson().object();
            o["angle"] = angle.toJson().object();
            return QJsonDocument(o);
        }
        static GeometricPropData fromJSON(const QJsonObject &o)
        {
            GeometricPropData d;
            d.plane = PlaneProperties::PlanePropData::fromJSON(o["plane"].toObject());
            d.angle = AngleProperties::AnglePropData::fromJSON(o["angle"].toObject());
            return d;
        }
    };

    GeometricPropData getData() const
    {
        GeometricPropData d;
        if (m_plane)
            d.plane = m_plane->getData();
        if (m_angle)
            d.angle = m_angle->getData();
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        if (m_plane && o.contains("plane"))
            m_plane->setfromJSON(o["plane"].toObject());
        if (m_angle && o.contains("angle"))
            m_angle->setfromJSON(o["angle"].toObject());
    }

signals:
    void planeChanged();
    void angleChanged();

private:
    QPointer<PlaneProperties> m_plane;
    QPointer<AngleProperties> m_angle;
};

#endif // GEOMETRICPROPERTIES_H
