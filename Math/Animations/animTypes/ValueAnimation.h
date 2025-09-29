#ifndef VALUEANIMATION_H
#define VALUEANIMATION_H

#include "../animation.h"
#include <QString>
#include <QList>

class ClickableMobject;

class ValueAnimation : public Animation
{
public:
    ValueAnimation(qreal startVal, qreal endVal, qreal startOffset, qreal duration);

    // Add mobject-property pairs to update on value change
    void addTarget(ClickableMobject *mobj, QString prop);

    void apply() override;

private:
    struct Target
    {
        ClickableMobject *mobj;
        QString prop;
    };
    qreal m_startVal;
    qreal m_endVal;
    QList<Target> m_targets;
};

#endif // VALUEANIMATION_H