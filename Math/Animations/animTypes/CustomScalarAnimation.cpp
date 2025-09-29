#include "CustomScalarAnimation.h"
#include "ClickableMobject.h"
#include "Managers/funcmap.h"

CustomScalarAnimation::CustomScalarAnimation(ClickableMobject *mobj, QString prop, qreal startVal, qreal targetVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj), m_prop(prop),
      m_startVal(startVal), m_targetVal(targetVal)
{
}

void CustomScalarAnimation::apply()
{
    if (!m_mobj || !m_mobj->getProperties())
    {
        return; // Safety check
    }
    auto func = FuncMap::ConnectFunc[m_prop];
    if (!func)
        return;
    qreal t = easedProgress();
    qreal diff = m_targetVal - m_startVal;
    qreal newVal = m_startVal + diff * t;
    func(m_mobj, newVal);
}
