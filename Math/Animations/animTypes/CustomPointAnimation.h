#ifndef CUSTOMPOINTANIMATION_H
#define CUSTOMPOINTANIMATION_H

#include "../animation.h"
#include <QString>
#include <QPointF>

class ClickableMobject;

// Custom Animation for QPointF properties
class CustomPointAnimation : public Animation
{
public:
    // Only requires target value - start value is captured automatically
    CustomPointAnimation(ClickableMobject *mobj, QString prop,
                         QPointF targetVal,
                         qreal startOffset, qreal duration);

    void apply() override;

protected:
    // Called when animation starts to capture current value
    void onStart() override;

private:
    ClickableMobject *m_mobj;
    QString m_prop;
    QPointF m_startVal;   // Captured automatically when animation starts
    QPointF m_targetVal;  // User-specified target value
    bool m_startCaptured; // Flag to ensure we capture start value only once
};

#endif // CUSTOMPOINTANIMATION_H