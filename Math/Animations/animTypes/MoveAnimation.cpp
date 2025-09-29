#include "MoveAnimation.h"
#include "ClickableMobject.h"

MoveAnimation::MoveAnimation(ClickableMobject *mobj, QPointF startPos, QPointF targetPos, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj),
      m_startPos(startPos), m_targetPos(targetPos)
{
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