#ifndef ANIMATION_H
#define ANIMATION_H

#include "ClickableMobject.h"
#include <qeasingcurve.h>

// Base abstract Animation class
class Animation
{
public:
    Animation(qreal startOffset, qreal duration,QEasingCurve::Type easingtype=QEasingCurve::InBounce);

    virtual ~Animation();

    // Set local time relative to this animation's startOffset
    void setLtime(qreal sceneTime);

    qreal progress() const;

    qreal easedProgress() const {
        return m_easing.valueForProgress(m_ltime);
    }

    // Each animation must implement how to apply progress to its target(s)
    virtual void apply() = 0;

    qreal getStartOffset() const;
    qreal getDuration() const;

protected:
    qreal m_ltime;           // Normalized local time (0 to 1)
    qreal m_startOffset;     // Absolute start time of animation in scene
    qreal m_duration;        // Duration of animation

    QEasingCurve m_easing = QEasingCurve::InOutQuad;
};

// Move Animation: moves a mobject from start to target position
class MoveAnimation : public Animation
{
public:
    MoveAnimation(ClickableMobject* mobj, QPointF startPos, QPointF targetPos,
                  qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject* m_mobj;
    QPointF m_startPos;
    QPointF m_targetPos;
};

// Create Animation: fades in mobject by increasing opacity from 0 to 1
class CreateAnimation : public Animation
{
public:
    CreateAnimation(ClickableMobject* mobj, qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject* m_mobj;
};

// Destroy Animation: fades out mobject by decreasing opacity from 1 to 0
class DestroyAnimation : public Animation
{
public:
    DestroyAnimation(ClickableMobject* mobj, qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject* m_mobj;
};

// Custom Animation for scalar properties
class CustomScalarAnimation : public Animation
{
public:
    CustomScalarAnimation(ClickableMobject* mobj, QString prop,
                          qreal startVal, qreal targetVal,
                          qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject* m_mobj;
    QString m_prop;
    qreal m_startVal;
    qreal m_targetVal;
};

// Custom Animation for QPointF properties
class CustomPointAnimation : public Animation
{
public:
    CustomPointAnimation(ClickableMobject* mobj, QString prop,
                         QPointF startVal, QPointF targetVal,
                         qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject* m_mobj;
    QString m_prop;
    QPointF m_startVal;
    QPointF m_targetVal;
};

class ValueAnimation : public Animation
{
public:
    ValueAnimation(qreal startVal, qreal endVal, qreal startOffset, qreal duration);

    // Add mobject-property pairs to update on value change
    void addTarget(ClickableMobject* mobj, QString prop);

    void apply() override;

private:
    struct Target {
        ClickableMobject* mobj;
        QString prop;
    };
    qreal m_startVal;
    qreal m_endVal;
    QList<Target> m_targets;
};


#endif // ANIMATION_H
