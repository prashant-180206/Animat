#include "VGroup.h"
#include <QSGGeometry>

/**
 * VGroup Implementation - Efficient group management for SimpleLine and SimpleDot
 *
 * This implementation focuses on managing groups of SimpleLine and SimpleDot objects
 * with shared visual properties (color, thickness/size) while maintaining individual positioning.
 * The key design principle is automatic property synchronization - when you add a member
 * to the group, it automatically adopts the group's shared properties.
 */

VGroup::VGroup(QQuickItem* parent,Scene *canvas)
    : ClickableMobject(canvas,parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

VGroup::~VGroup()
{
    clearMembers();
}

// Add SimpleLine member with automatic property sync
void VGroup::addMember(SimpleLine* line)
{
    if (!line || m_members.contains(line)) {
        return;
    }

    m_members.append(line);
    line->setParentItem(this);  // Make it a child for rendering hierarchy
    
    // Apply shared properties immediately
    syncMemberToSharedProperties(line);
    
    // Connect to update when member properties change
    connect(line, &QQuickItem::xChanged, this, [this] { m_needsUpdate = true; update(); });
    connect(line, &QQuickItem::yChanged, this, [this] { m_needsUpdate = true; update(); });
    
    m_needsUpdate = true;
    update();
    emit memberAdded(line);
}

// Add SimpleDot member with automatic property sync
void VGroup::addMember(SimpleDot* dot)
{
    if (!dot || m_members.contains(dot)) {
        return;
    }

    m_members.append(dot);
    dot->setParentItem(this);  // Make it a child for rendering hierarchy
    
    // Apply shared properties immediately
    syncMemberToSharedProperties(dot);
    
    // Connect to update when member properties change
    connect(dot, &QQuickItem::xChanged, this, [this] { m_needsUpdate = true; update(); });
    connect(dot, &QQuickItem::yChanged, this, [this] { m_needsUpdate = true; update(); });
    
    m_needsUpdate = true;
    update();
    emit memberAdded(dot);
}

void VGroup::removeMember(QQuickItem* mobject)
{
    if (m_members.removeOne(mobject)) {
        mobject->setParentItem(nullptr);
        mobject->disconnect(this);  // Disconnect all signals
        m_needsUpdate = true;
        update();
        emit memberRemoved(mobject);
    }
}

void VGroup::clearMembers()
{
    for (QQuickItem* member : std::as_const(m_members)) {
        member->setParentItem(nullptr);
        member->disconnect(this);
    }
    m_members.clear();
    m_needsUpdate = true;
    update();
}

int VGroup::getMemberCount() const
{
    return m_members.size();
}

// Shared property getters
QColor VGroup::sharedColor() const
{
    return m_sharedColor;
}

qreal VGroup::sharedThickness() const
{
    return m_sharedThickness;
}

qreal VGroup::sharedSize() const
{
    return m_sharedSize;
}

// Shared property setters with automatic synchronization
void VGroup::setSharedColor(const QColor& color)
{
    if (m_sharedColor != color) {
        m_sharedColor = color;
        syncAllMembersToSharedProperties();
        m_needsUpdate = true;
        update();
        emit sharedColorChanged();
    }
}

void VGroup::setSharedThickness(qreal thickness)
{
    if (qAbs(m_sharedThickness - thickness) > 0.01) {
        m_sharedThickness = thickness;
        // Apply to all SimpleLine members
        for (QQuickItem* member : m_members) {
            if (SimpleLine* line = qobject_cast<SimpleLine*>(member)) {
                line->setThickness(thickness);
            }
        }
        m_needsUpdate = true;
        update();
        emit sharedThicknessChanged();
    }
}

void VGroup::setSharedSize(qreal size)
{
    if (qAbs(m_sharedSize - size) > 0.01) {
        m_sharedSize = size;
        // Apply to all SimpleDot members
        for (QQuickItem* member : m_members) {
            if (SimpleDot* dot = qobject_cast<SimpleDot*>(member)) {
                dot->setSize(size);
            }
        }
        m_needsUpdate = true;
        update();
        emit sharedSizeChanged();
    }
}

// Sync single member to shared properties
void VGroup::syncMemberToSharedProperties(QQuickItem* member)
{
    if (SimpleLine* line = qobject_cast<SimpleLine*>(member)) {
        line->setColor(m_sharedColor);
        line->setThickness(m_sharedThickness);
    } else if (SimpleDot* dot = qobject_cast<SimpleDot*>(member)) {
        dot->setColor(m_sharedColor);
        dot->setSize(m_sharedSize);
    }
}

// Sync all members to shared properties
void VGroup::syncAllMembersToSharedProperties()
{
    for (QQuickItem* member : m_members) {
        syncMemberToSharedProperties(member);
    }
}

// Apply size or thickness based on member type
void VGroup::applySizeOrThickness(QQuickItem* member)
{
    if (SimpleLine* line = qobject_cast<SimpleLine*>(member)) {
        line->setThickness(m_sharedThickness);
    } else if (SimpleDot* dot = qobject_cast<SimpleDot*>(member)) {
        dot->setSize(m_sharedSize);
    }
}

// Efficient batch rendering - let members render themselves within group context
QSGNode* VGroup::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data)
{
    // For VGroup, we don't need custom rendering since members render themselves
    // The group just manages positioning and shared properties
    // Members are child items so Qt handles their rendering automatically
    
    if (m_needsUpdate) {
        m_needsUpdate = false;
        // Group bounds update could go here if needed
    }
    
    return oldNode;  // No custom geometry needed
}
