#include "circleproperties.h"

CircleProperties::CircleProperties(QObject *parent)
    : QObject{parent}
{}

qreal CircleProperties::radius() const { return m_radius; }

void CircleProperties::setRadius(qreal radius) {
    if (!qFuzzyCompare(m_radius, radius)) {
        m_radius = radius;
        emit radiusChanged();
    }
}

void CircleProperties::setSegments(int segments)
{
    if (m_segments != segments) {
        m_segments = segments;
        emit segmentsChanged();
    }
}
