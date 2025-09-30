#ifndef CUSTOMSCALARANIMATION_H
#define CUSTOMSCALARANIMATION_H

#include "../animation.h"
#include <QString>

class ClickableMobject;

// Custom Animation for scalar properties
class CustomScalarAnimation : public Animation
{
public:
    // Only requires target value - start value is captured automatically
    CustomScalarAnimation(ClickableMobject *mobj, QString prop,
                          qreal targetVal,
                          qreal duration, QEasingCurve::Type easingType = QEasingCurve::InOutQuad);

    void apply() override;

protected:
    // Called when animation starts to capture current value
    void onStart() override;

private:
    ClickableMobject *m_mobj;
    QString m_prop;
    qreal m_startVal;     // Captured automatically when animation starts
    qreal m_targetVal;    // User-specified target value
    bool m_startCaptured; // Flag to ensure we capture start value only once
};

#endif // CUSTOMSCALARANIMATION_H