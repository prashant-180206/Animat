#ifndef ANIMATION_H
#define ANIMATION_H

#include "Math/Mobjects/Base/ClickableMobject.h"
#include <qeasingcurve.h>

// Base abstract Animation class
class Animation
{
public:
    Animation(qreal startOffset, qreal duration, QEasingCurve::Type easingtype = QEasingCurve::InBounce);

    virtual ~Animation();

    // Set local time relative to this animation's startOffset
    void setLtime(qreal sceneTime = 0)
    {
        qreal localTime = sceneTime - m_startOffset;
        if (localTime < 0)
        {
            m_ltime = 0;
        }
        else if (localTime > m_duration)
        {
            m_ltime = 1;
        }
        else
        {
            m_ltime = localTime / m_duration;

            // Call onStart when animation begins (transition from not started to started)
            if (!m_started && localTime >= 0)
            {
                m_started = true;
                onStart();
            }
        }
    }

    qreal progress() const;

    qreal easedProgress() const
    {
        return m_easing.valueForProgress(m_ltime);
    }

    // Each animation must implement how to apply progress to its target(s)
    virtual void apply() = 0;

    qreal getStartOffset() const;
    qreal getDuration() const;

protected:
    // Called when animation starts (first time it becomes active)
    virtual void onStart() {}

    qreal m_ltime = 0;      // Normalized local time (0 to 1)
    qreal m_startOffset;    // Absolute start time of animation in scene
    qreal m_duration;       // Duration of animation
    bool m_started = false; // Flag to track if animation has started

    QEasingCurve m_easing = QEasingCurve::InOutQuad;
};

#endif // ANIMATION_H
