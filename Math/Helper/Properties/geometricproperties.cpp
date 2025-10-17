#include "geometricproperties.h"
#include "Geometric/planeproperties.h"
#include "Geometric/angleproperties.h"

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

void GeometricProperties::setAngle(AngleProperties *angle)
{
    if (m_angle.data() != angle)
    {
        m_angle = angle;
        emit angleChanged();
    }
}