#include "animation.h"

Animation::Animation(qreal startOffset, qreal duration, QEasingCurve::Type easingtype)
    : m_startOffset(startOffset), m_duration(duration), m_ltime(0) {}

Animation::~Animation() {}

qreal Animation::progress() const { return m_ltime; }

qreal Animation::getStartOffset() const { return m_startOffset; }

qreal Animation::getDuration() const { return m_duration; }
