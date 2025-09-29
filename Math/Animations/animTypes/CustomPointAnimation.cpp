#include "CustomPointAnimation.h"
#include "ClickableMobject.h"
#include "Managers/funcmap.h"


CustomPointAnimation::CustomPointAnimation(ClickableMobject *mobj, QString prop, QPointF startVal, QPointF targetVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj), m_prop(prop),
      m_startVal(startVal), m_targetVal(targetVal)
{
}

void CustomPointAnimation::apply()
{
    if (!m_mobj || !m_mobj->getProperties())
    {
        return; // Safety check
    }
    auto func = FuncMap::PtConnectFunc[m_prop];
    if (!func)
        return;
    qreal t = easedProgress();
    QPointF diff = m_targetVal - m_startVal;
    QPointF newVal = m_startVal + diff * t;
    func(m_mobj, newVal);
}
