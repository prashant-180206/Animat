#include "CreateAnimation.h"
#include "Math/Mobjects/Base/ClickableMobject.h"

CreateAnimation::CreateAnimation(ClickableMobject *mobj, qreal duration, QEasingCurve::Type easingType)
    : Animation(duration, easingType), m_mobj(mobj)
{
}

void CreateAnimation::apply()
{
    if (!m_mobj || !m_mobj->getProperties() || !m_mobj->getProperties()->base())
    {
        return; // Safety check
    }
    qreal newOpacity = easedProgress(); // from 0 to 1 with easing
    m_mobj->getProperties()->base()->setOpacity(newOpacity);
}
