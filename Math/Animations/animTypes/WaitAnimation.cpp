#include "WaitAnimation.h"

WaitAnimation::WaitAnimation(qreal duration, QEasingCurve::Type easingType)
    : Animation(duration, easingType)
{
}

void WaitAnimation::apply()
{
    // Wait animation does nothing - it just consumes time
    // This creates a controllable gap/delay between other animations
}