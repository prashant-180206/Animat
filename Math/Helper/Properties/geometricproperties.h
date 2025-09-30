#ifndef GEOMETRICPROPERTIES_H
#define GEOMETRICPROPERTIES_H

#include <QObject>
#include <QQmlEngine>
#include <qqmlintegration.h>
#include <QPointer>
#include "planeproperties.h"

class GeometricProperties : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(PlaneProperties *plane READ plane WRITE setPlane NOTIFY planeChanged)

public:
    explicit GeometricProperties(QObject *parent = nullptr);

    PlaneProperties *plane() const { return m_plane.data(); }
    void setPlane(PlaneProperties *plane);

signals:
    void planeChanged();

private:
    QPointer<PlaneProperties> m_plane;
};

#endif // GEOMETRICPROPERTIES_H