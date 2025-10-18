#include "DestroyAnimation.h"
#include "Math/Mobjects/Base/ClickableMobject.h"

DestroyAnimation::DestroyAnimation(ClickableMobject *mobj, qreal duration, QEasingCurve::Type easingType)
    : Animation(duration, easingType), m_mobj(mobj)
{
}

void DestroyAnimation::apply()
{
    if (!m_mobj || !m_mobj->getProperties() || !m_mobj->getProperties()->base())
    {
        return; // Safety check
    }
    qreal newOpacity = 1.0 - easedProgress(); // from 1 to 0 with easing
    m_mobj->getProperties()->base()->setOpacity(newOpacity);
}
