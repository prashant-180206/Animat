#include "group.h"

Group::Group(Scene* canvas, QQuickItem *parent)
    : ClickableMobject(canvas, parent), m_spacing(0)
{
    properties["Name"]="Group";
}

Group::~Group()
{
    const auto childrenList = childItems();
    for (QQuickItem *child : childrenList) {
        child->deleteLater();
    }
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
    qreal maxWidth = 0;  // Track max width of children
    const auto childrenList = childItems();
    for (QQuickItem *child : childrenList) {
        if (!child->isVisible())
            continue;

        child->setY(yPos);
        yPos += child->height() + m_spacing;

        qreal childWidth = child->width();
        if (childWidth > maxWidth)
            maxWidth = childWidth;
    }
    qreal h = yPos > 0 ? yPos - m_spacing : 0;
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

// void Group::setCenter(qreal xval, qreal yval)
// {
//     qDebug()<<"GROUP CENTER CALLED";
//     ClickableMobject::setCenter(xval,yval);

// }
