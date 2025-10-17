#ifndef CREATEANIMATION_H
#define CREATEANIMATION_H

#include "../animation.h"

class ClickableMobject;

// Create Animation: fades in mobject by increasing opacity from 0 to 1
class CreateAnimation : public Animation
{
public:
    CreateAnimation(ClickableMobject *mobj, qreal duration, QEasingCurve::Type easingType = QEasingCurve::InOutQuad);

    void apply() override;

private:
    ClickableMobject *m_mobj;
};

#endif // CREATEANIMATION_H
