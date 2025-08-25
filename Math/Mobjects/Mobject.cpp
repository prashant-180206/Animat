#include "Mobject.h"

Mobject::Mobject(QQuickItem* parent) : QQuickItem(parent), m_center()
{
    if (parent) {
        QPointF c = this->mapToItem(parent, QPointF(0,0));
        setCenter(c);
    }
}

QPointF Mobject::center() const
{
    return m_center;
}

QString Mobject::id() const
{
    return m_id;
}

void Mobject::setCenter(const QPointF& v)
{
    if (m_center != v) {
        m_center = v;
        // Update visual position if needed
        setPosition(m_center);
        emit centerChanged();
    }
}

void Mobject::setId(QString i)
{
    if (m_id != i) {
        m_id = i;
        emit idChanged();
    }
}
