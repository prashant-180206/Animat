#include "mproperties.h"

MProperties::MProperties(QObject *parent)
    : QObject(parent)
{
    // Initialize all property objects to avoid null pointer access
    // m_base = new BaseProperties(this);
    // m_circle = new CircleProperties(this);
    // m_curve = new CurveProperties(this);
    // m_line = new LineProperties(this);
    // m_polygon = new PolygonProperties(this);
}

void MProperties::setBase(BaseProperties *base) {
    if (m_base == base)
        return;
    m_base = base;
    emit baseChanged();
}

void MProperties::setCircle(CircleProperties *circle) {
    if (m_circle == circle)
        return;
    m_circle = circle;
    emit circleChanged();
}

void MProperties::setCurve(CurveProperties *curve) {
    if (m_curve == curve)
        return;
    m_curve = curve;
    emit curveChanged();
}

void MProperties::setLine(LineProperties *line) {
    if (m_line == line)
        return;
    m_line = line;
    emit lineChanged();
}

void MProperties::setPolygon(PolygonProperties *polygon) {
    if (m_polygon == polygon)
        return;
    m_polygon = polygon;
    emit polygonChanged();
}
