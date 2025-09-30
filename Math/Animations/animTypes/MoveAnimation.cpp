#include "MoveAnimation.h"
#include "Math/Mobjects/Base/ClickableMobject.h"

MoveAnimation::MoveAnimation(ClickableMobject *mobj, QPointF targetPos, qreal duration, QEasingCurve::Type easingType)
    : Animation(duration, easingType), m_mobj(mobj),
      m_startPos(QPointF(0, 0)), m_targetPos(targetPos), m_startCaptured(false)
{
}

void MoveAnimation::onStart()
{
    if (!m_startCaptured && m_mobj && m_mobj->getProperties() && m_mobj->getProperties()->base())
    {
        // Capture the current position as the starting position
        m_startPos = m_mobj->getProperties()->base()->pos();
        m_startCaptured = true;
    }

    // Call parent implementation
    Animation::onStart();
}

void MoveAnimation::apply()
{
    if (!m_mobj || !m_mobj->getProperties() || !m_mobj->getProperties()->base())
    {
        return; // Safety check
    }
    qreal t = easedProgress();
    QPointF diff = m_targetPos - m_startPos;
    QPointF newPos = m_startPos + diff * t;
    m_mobj->getProperties()->base()->setPos(newPos);
}