#ifndef GEOMETRICPROPERTIES_H
#define GEOMETRICPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qqmlintegration.h>
#include <QPointer>
#include "Geometric/planeproperties.h"

class GeometricProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(PlaneProperties *plane READ plane WRITE setPlane NOTIFY planeChanged)

public:
    explicit GeometricProperties(QObject *parent = nullptr);

    PlaneProperties *plane() const { return m_plane.data(); }
    void setPlane(PlaneProperties *plane);

    struct GeometricPropData
    {
        PlaneProperties::PlanePropData plane;
        QJsonDocument toJson() const
        {
            QJsonObject o;
            o["plane"] = plane.toJson().object();
            return QJsonDocument(o);
        }
        static GeometricPropData fromJSON(const QJsonObject &o)
        {
            GeometricPropData d;
            d.plane = PlaneProperties::PlanePropData::fromJSON(o["plane"].toObject());
            return d;
        }
    };

    GeometricPropData getData() const
    {
        GeometricPropData d;
        if (m_plane)
            d.plane = m_plane->getData();
        return d;
    }

    void setfromJSON(const QJsonObject &o)
    {
        if (m_plane && o.contains("plane"))
            m_plane->setfromJSON(o["plane"].toObject());
    }

signals:
    void planeChanged();

private:
    QPointer<PlaneProperties> m_plane;
};

#endif // GEOMETRICPROPERTIES_H
