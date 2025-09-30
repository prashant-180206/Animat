#include "CreateAnimation.h"
#include "Math/Mobjects/Base/ClickableMobject.h"

CreateAnimation::CreateAnimation(ClickableMobject *mobj, qreal startOffset, qreal duration)
    : Animation(startOffset, duration), m_mobj(mobj)
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

    // When animation starts (opacity > 0) and zindex change is enabled, restore zindex
    if (newOpacity > 0.0 && m_changeZIndexOnCreate)
    {
        m_mobj->getProperties()->base()->setZindex(m_createdZIndex);
    }
}