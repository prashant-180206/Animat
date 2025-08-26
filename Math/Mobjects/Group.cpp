#include "group.h"

Group::Group(Scene* canvas,QQuickItem *parent)
    : Mobject(canvas,parent), m_spacing(0)
{
}

int Group::spacing() const
{
    return m_spacing;
}

void Group::setSpacing(int spacing)
{
    if (m_spacing == spacing)
        return;

    m_spacing = spacing;
    emit spacingChanged();
}

void Group::arrange()
{
    qreal yPos = 0;
    // Only layout direct children of this group
    const auto childrenList = childItems();

    for (QQuickItem *child : childrenList) {
        if (!child->isVisible())
            continue;

        child->setY(yPos);
        yPos += child->height() + m_spacing;
    }

    setHeight(yPos > 0 ? yPos - m_spacing : 0);
}

void Group::addMember(Mobject *item)
{
    if (!item)
        return;

    item->setParentItem(this);  // Make this Group the visual parent
    arrange();                  // Rearrange after adding
}

void Group::removeMember(Mobject *item)
{
    if (!item)
        return;

    // Only remove if this is the parent
    if (item->parentItem() == this) {
        item->setParentItem(nullptr);
        arrange();  // Rearrange after removal
    }
}
