#include "WaitAnimation.h"

WaitAnimation::WaitAnimation(qreal startOffset, qreal duration)
    : Animation(startOffset, duration)
{
}

void WaitAnimation::apply()
{
    // Wait animation does nothing - it just consumes time
    // This creates a controllable gap/delay between other animations
}