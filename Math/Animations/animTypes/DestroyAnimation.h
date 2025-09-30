#ifndef DESTROYANIMATION_H
#define DESTROYANIMATION_H

#include "../animation.h"

class ClickableMobject;

// Destroy Animation: fades out mobject by decreasing opacity from 1 to 0
class DestroyAnimation : public Animation
{
public:
    DestroyAnimation(ClickableMobject *mobj, qreal duration, QEasingCurve::Type easingType = QEasingCurve::InOutQuad);

    void apply() override;

protected:
    // Called when animation starts to capture original z-index
    void onStart() override;

private:
    ClickableMobject *m_mobj;
    qreal m_originalZIndex; // Store the original z-index to restore later
};

#endif // DESTROYANIMATION_H