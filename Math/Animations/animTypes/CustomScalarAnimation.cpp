#include "CustomScalarAnimation.h"
#include "ClickableMobject.h"
#include "Managers/funcmap.h"

CustomScalarAnimation::CustomScalarAnimation(ClickableMobject *mobj, QString prop, qreal targetVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj), m_prop(prop),
      m_startVal(0.0), m_targetVal(targetVal), m_startCaptured(false)
{
}

void CustomScalarAnimation::onStart()
{
    if (!m_startCaptured && m_mobj && m_mobj->getProperties())
    {
        // Capture the current value as the starting value
        auto getterFunc = FuncMap::GetterFunc[m_prop];
        if (getterFunc)
        {
            m_startVal = getterFunc(m_mobj);
            m_startCaptured = true;
        }
    }

    // Call parent implementation
    Animation::onStart();
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
