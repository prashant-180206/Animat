#include "angleproperties.h"

AngleProperties::AngleProperties(QObject *parent)
    : QObject(parent)
{
    setParent(parent);
}

void AngleProperties::setAngle(qreal angle)
{
    if (qAbs(m_angle - angle) > 1e-6)
    {
        m_angle = angle;
        emit angleChanged();
    }
}

void AngleProperties::setStartVectorLength(qreal length)
{
    if (qAbs(m_startVectorLength - length) > 1e-6)
    {
        m_startVectorLength = length;
        emit startVectorLengthChanged();
    }
}

void AngleProperties::setEndVectorLength(qreal length)
{
    if (qAbs(m_endVectorLength - length) > 1e-6)
    {
        m_endVectorLength = length;
        emit endVectorLengthChanged();
    }
}

void AngleProperties::setArcColor(const QColor &color)
{
    if (m_arcColor != color)
    {
        m_arcColor = color;
        emit arcColorChanged();
    }
}

void AngleProperties::setArcRadius(qreal radius)
{
    if (qAbs(m_arcRadius - radius) > 1e-6)
    {
        m_arcRadius = radius;
        emit arcRadiusChanged();
    }
}

void AngleProperties::setArcThickness(qreal thickness)
{
    if (qAbs(m_arcThickness - thickness) > 1e-6)
    {
        m_arcThickness = thickness;
        emit arcThicknessChanged();
    }
}