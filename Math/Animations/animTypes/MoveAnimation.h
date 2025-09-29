#ifndef MOVEANIMATION_H
#define MOVEANIMATION_H

#include "../animation.h"
#include <QPointF>

class ClickableMobject;

// Move Animation: moves a mobject from start to target position
class MoveAnimation : public Animation
{
public:
    MoveAnimation(ClickableMobject *mobj, QPointF startPos, QPointF targetPos,
                  qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject *m_mobj;
    QPointF m_startPos;
    QPointF m_targetPos;
};

#endif // MOVEANIMATION_H