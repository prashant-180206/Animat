#include "geometricproperties.h"
#include "Geometric/planeproperties.h"

GeometricProperties::GeometricProperties(QObject *parent)
    : QObject(parent)
{
}

void GeometricProperties::setPlane(PlaneProperties *plane)
{
    if (m_plane.data() != plane)
    {
        m_plane = plane;
        emit planeChanged();
    }
}