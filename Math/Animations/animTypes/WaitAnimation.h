#ifndef WAITANIMATION_H
#define WAITANIMATION_H

#include "../animation.h"

// Wait Animation: provides a controllable delay/gap between animations
class WaitAnimation : public Animation
{
public:
    WaitAnimation(qreal startOffset, qreal duration);

    void apply() override;

private:
    // No private members needed - this animation just consumes time
};

#endif // WAITANIMATION_H