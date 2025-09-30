#ifndef MOVEANIMATION_H
#define MOVEANIMATION_H

#include "../animation.h"
#include <QPointF>

class ClickableMobject;

// Move Animation: moves a mobject from current position to target position
class MoveAnimation : public Animation
{
public:
    // Only requires target position - start position is captured automatically
    MoveAnimation(ClickableMobject *mobj, QPointF targetPos,
                  qreal duration, QEasingCurve::Type easingType = QEasingCurve::InOutQuad);

    void apply() override;

protected:
    // Called when animation starts to capture current position
    void onStart() override;

private:
    ClickableMobject *m_mobj;
    QPointF m_startPos;   // Captured automatically when animation starts
    QPointF m_targetPos;  // User-specified target position
    bool m_startCaptured; // Flag to ensure we capture start position only once
};

#endif // MOVEANIMATION_H