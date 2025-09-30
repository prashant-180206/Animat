#include "ValueAnimation.h"
#include "Math/Mobjects/Base/ClickableMobject.h"
#include "Managers/funcmap.h"

ValueAnimation::ValueAnimation(qreal startVal, qreal endVal, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_startVal(startVal), m_endVal(endVal) {}

void ValueAnimation::addTarget(ClickableMobject *mobj, QString prop)
{
    m_targets.push_back({mobj, prop});
}

void ValueAnimation::apply()
{
    qreal t = easedProgress();
    qreal currentVal = m_startVal + (m_endVal - m_startVal) * t;
    for (const auto &target : std::as_const(m_targets))
    {
        if (!target.mobj || !target.mobj->getProperties())
        {
            continue; // Safety check
        }
        auto func = FuncMap::ConnectFunc[target.prop];
        if (func)
        {
            func(target.mobj, currentVal);
        }
    }
}
