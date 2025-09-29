#ifndef CUSTOMSCALARANIMATION_H
#define CUSTOMSCALARANIMATION_H

#include "../animation.h"
#include <QString>

class ClickableMobject;

// Custom Animation for scalar properties
class CustomScalarAnimation : public Animation
{
public:
    CustomScalarAnimation(ClickableMobject *mobj, QString prop,
                          qreal startVal, qreal targetVal,
                          qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject *m_mobj;
    QString m_prop;
    qreal m_startVal;
    qreal m_targetVal;
};

#endif // CUSTOMSCALARANIMATION_H