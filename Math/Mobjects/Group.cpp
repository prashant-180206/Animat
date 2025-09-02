#include "group.h"

Group::Group(Scene* canvas, QQuickItem *parent)
    : ClickableMobject(canvas, parent)
{
    properties->setName("Group");
}

Group::~Group()
{
    const auto childrenList = childItems();
    for (QQuickItem *child : childrenList) {
        child->deleteLater();
    }
}



void Group::arrange()
{
    qreal yPos = 0;
    qreal maxWidth = 0;  // Track max width of children
    const auto childrenList = childItems();
    for (QQuickItem *child : childrenList) {
        if (!child->isVisible())
            continue;

        child->setY(yPos);
        yPos += child->height() ;

        qreal childWidth = child->width();
        if (childWidth > maxWidth)
            maxWidth = childWidth;
    }
    qreal h = yPos > 0 ? yPos  : 0;
    setSize(h, maxWidth);
}

void Group::addMember(Mobject *item)
{
    if (!item)
        return;
    item->setParentItem(this);
    item->setZ(this->z() - 1);
    arrange();
}

void Group::removeMember(Mobject *item)
{
    if (!item)
        return;
    if (item->parentItem() == this) {
        item->setParentItem(nullptr);
        arrange();
    }
}

