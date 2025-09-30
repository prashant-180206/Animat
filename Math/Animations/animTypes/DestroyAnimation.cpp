#include "DestroyAnimation.h"
#include "Math/Mobjects/Base/ClickableMobject.h"

DestroyAnimation::DestroyAnimation(ClickableMobject *mobj, qreal duration, QEasingCurve::Type easingType)
    : Animation(duration, easingType), m_mobj(mobj), m_originalZIndex(0)
{
}

void DestroyAnimation::onStart()
{
    // Capture the original z-index when animation starts
    if (m_mobj && m_mobj->getProperties() && m_mobj->getProperties()->base())
    {
        m_originalZIndex = m_mobj->getProperties()->base()->zindex();
    }
}

void DestroyAnimation::apply()
{
    if (!m_mobj || !m_mobj->getProperties() || !m_mobj->getProperties()->base())
    {
        return; // Safety check
    }
    if (easedProgress() > 0.9)
    {
        m_mobj->getProperties()->base()->setZindex(-1);
    }
    else
    {
        // Restore to original z-index instead of hardcoded 1
        m_mobj->getProperties()->base()->setZindex(m_originalZIndex);
    }

    qreal newOpacity = 1.0 - easedProgress(); // from 1 to 0 with easing
    m_mobj->getProperties()->base()->setOpacity(newOpacity);
}