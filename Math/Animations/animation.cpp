#include "animation.h"

Animation::Animation(qreal duration, QEasingCurve::Type easingtype)
    : m_duration(duration), m_ltime(0) {}

Animation::~Animation() {}

qreal Animation::progress() const { return m_ltime; }

qreal Animation::getDuration() const { return m_duration; }
