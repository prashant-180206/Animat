#ifndef VGROUP_H
#define VGROUP_H

/**
 * VGroup (Virtual Group) - A container for SimpleLine and SimpleDot objects with shared properties
 *
 * Purpose: Groups multiple SimpleLine or SimpleDot objects that share the same visual properties
 * (color, thickness/size) but have different positions. Renders all members efficiently as a
 * single unit with automatic property synchronization.
 *
 * Key Features:
 * - Shared color property applied to all members
 * - Shared thickness (for lines) or size (for dots) property
 * - Individual member positioning
 * - Efficient batch rendering of all members
 * - Automatic property synchronization through Q_PROPERTY bindings
 * - NOT registered in Scene (use programmatically only)
 *
 * Example Usage:
 * ```cpp
 * // Create VGroup for particles
 * VGroup* particleGroup = new VGroup(canvas);
 * particleGroup->setSharedColor(Qt::red);
 * particleGroup->setSharedSize(8.0);
 *
 * // Add individual particles with different positions
 * for (int i = 0; i < 10; ++i) {
 *     SimpleDot* particle = new SimpleDot(canvas);
 *     particle->setPos(QPointF(i * 20, sin(i) * 10));
 *     particleGroup->addMember(particle);  // Automatically applies shared properties
 * }
 *
 * // Create VGroup for line segments
 * VGroup* lineGroup = new VGroup(canvas);
 * lineGroup->setSharedColor(Qt::blue);
 * lineGroup->setSharedThickness(3.0);
 *
 * for (int i = 0; i < 5; ++i) {
 *     SimpleLine* line = new SimpleLine(canvas);
 *     line->setP1(QPointF(i * 30, 0));
 *     line->setP2(QPointF(i * 30, 50));
 *     lineGroup->addMember(line);  // Automatically applies shared properties
 * }
 * ```
 */

#include "Math/Mobjects/Base/ClickableMobject.h"
#include "Math/Mobjects/Simple/SimpleLine.h"
#include "Math/Mobjects/Simple/SimpleDot.h"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QVector>

class VGroup : public ClickableMobject
{
    Q_OBJECT
    QML_ELEMENT

    // Shared properties for all members
    Q_PROPERTY(QColor sharedColor READ sharedColor WRITE setSharedColor NOTIFY sharedColorChanged)
    Q_PROPERTY(qreal sharedThickness READ sharedThickness WRITE setSharedThickness NOTIFY sharedThicknessChanged)
    Q_PROPERTY(qreal sharedSize READ sharedSize WRITE setSharedSize NOTIFY sharedSizeChanged)

public:
    explicit VGroup(QQuickItem *parent = nullptr, Scene *canvas = nullptr);
    virtual ~VGroup();

    // Member management for SimpleLine and SimpleDot
    Q_INVOKABLE void addMember(SimpleLine *line);
    Q_INVOKABLE void addMember(SimpleDot *dot);
    Q_INVOKABLE void removeMember(QQuickItem *mobject);
    Q_INVOKABLE void clearMembers();
    Q_INVOKABLE int getMemberCount() const;

    // Shared property getters
    QColor sharedColor() const;
    qreal sharedThickness() const;
    qreal sharedSize() const;

    // Shared property setters with automatic synchronization
    void setSharedColor(const QColor &color);
    void setSharedThickness(qreal thickness);
    void setSharedSize(qreal size);

signals:
    void sharedColorChanged();
    void sharedThicknessChanged();
    void sharedSizeChanged();
    void memberAdded(QQuickItem *member);
    void memberRemoved(QQuickItem *member);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private slots:
    void syncMemberToSharedProperties(QQuickItem *member);

private:
    void syncAllMembersToSharedProperties();
    void applySizeOrThickness(QQuickItem *member);

    // Members
    QVector<QQuickItem *> m_members;

    // Shared properties
    QColor m_sharedColor = Qt::black;
    qreal m_sharedThickness = 2.0; // For SimpleLine objects
    qreal m_sharedSize = 6.0;      // For SimpleDot objects

    // Render cache
    QSGGeometryNode *m_geometryNode = nullptr;
    QSGFlatColorMaterial *m_material = nullptr;
    bool m_needsUpdate = true;
};

#endif // VGROUP_H
