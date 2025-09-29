#include "CustomPointAnimation.h"
#include "ClickableMobject.h"
#include "Managers/funcmap.h"

CustomPointAnimation::CustomPointAnimation(ClickableMobject *mobj, QString prop, QPointF targetVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj), m_prop(prop),
      m_startVal(QPointF(0, 0)), m_targetVal(targetVal), m_startCaptured(false)
{
}

void CustomPointAnimation::onStart()
{
    if (!m_startCaptured && m_mobj && m_mobj->getProperties())
    {
        // Capture the current value as the starting value
        auto getterFunc = FuncMap::PtGetterFunc[m_prop];
        if (getterFunc)
        {
            m_startVal = getterFunc(m_mobj);
            m_startCaptured = true;
        }
    }

    // Call parent implementation
    Animation::onStart();
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
