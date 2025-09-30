#ifndef CREATEANIMATION_H
#define CREATEANIMATION_H

#include "../animation.h"

class ClickableMobject;

// Create Animation: fades in mobject by increasing opacity from 0 to 1
class CreateAnimation : public Animation
{
public:
    CreateAnimation(ClickableMobject *mobj, qreal startOffset, qreal duration);

    void apply() override;

    // Property to control zindex behavior
    void setCreatedZIndex(qreal zindex) { m_createdZIndex = zindex; }
    qreal createdZIndex() const { return m_createdZIndex; }

    void setChangeZIndexOnCreate(bool change) { m_changeZIndexOnCreate = change; }
    bool changeZIndexOnCreate() const { return m_changeZIndexOnCreate; }

private:
    ClickableMobject *m_mobj;
    qreal m_createdZIndex = 0.0;        // Default zindex when created/restored
    bool m_changeZIndexOnCreate = true; // Whether to change zindex when created
};

#endif // CREATEANIMATION_H