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
    CustomPointAnimation(ClickableMobject *mobj, QString prop,
                         QPointF startVal, QPointF targetVal,
                         qreal startOffset, qreal duration);

    void apply() override;

private:
    ClickableMobject *m_mobj;
    QString m_prop;
    QPointF m_startVal;
    QPointF m_targetVal;
};

#endif // CUSTOMPOINTANIMATION_H