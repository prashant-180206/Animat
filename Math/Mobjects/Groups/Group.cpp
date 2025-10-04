#include "group.h"

Group::Group(Scene* canvas, QQuickItem *parent)
    : ClickableMobject(canvas, parent)
{
    properties->base()->setName("Group");
}

Group::~Group()
{
    const auto childrenList = childItems();
    for (QQuickItem *child : childrenList) {
        child->deleteLater();
    }
}

void Group::addMember(Mobject *item)
{
    if (!item)
        return;
    item->setParentItem(this);
    item->setZ(this->z() - 0.1);
    child.append(item);
}

void Group::removeAllMembers()
{
    for (auto p : std::as_const(child)) {
        if (p) {
            p->setParentItem(nullptr);  // Detach from this group
            p->deleteLater();           // Safe deferred deletion
        }
    }
    child.clear();  // Clear the list tracking members
}

