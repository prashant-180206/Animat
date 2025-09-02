#ifndef GROUP_H
#define GROUP_H

#include <QQuickItem>
#include <QRectF>
#include "Math/Helper/ClickableMobject.h"
#include "Mobject.h"

class Group : public ClickableMobject
{
    Q_OBJECT


public:
    explicit Group(Scene* canvas, QQuickItem *parent = nullptr);
    ~Group();  // Destructor to manage children destruction

    void arrange();
    void addMember(Mobject *item);
    void removeMember(Mobject *item);

private:
    QRectF m_membersBoundingRect;
};

#endif // GROUP_H
