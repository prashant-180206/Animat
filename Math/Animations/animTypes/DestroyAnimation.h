#ifndef DESTROYANIMATION_H
#define DESTROYANIMATION_H

#include "../animation.h"

class ClickableMobject;

// Destroy Animation: fades out mobject by decreasing opacity from 1 to 0
class DestroyAnimation : public Animation
{
public:
    DestroyAnimation(ClickableMobject *mobj, qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject *m_mobj;
};

#endif // DESTROYANIMATION_H