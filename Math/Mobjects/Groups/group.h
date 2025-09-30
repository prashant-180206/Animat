#ifndef GROUP_H
#define GROUP_H

#include <QQuickItem>
#include <QRectF>
#include "Math/Mobjects/Base/ClickableMobject.h"
#include "Mobjects/Base/Mobject.h"

class Group : public ClickableMobject
{
    Q_OBJECT

public:
    explicit Group(Scene *canvas, QQuickItem *parent = nullptr);
    ~Group(); // Destructor to manage children destruction

    void addMember(Mobject *item);
    void removeAllMembers();

private:
    QList<Mobject *> child;
    QRectF m_membersBoundingRect;
};

#endif // GROUP_H
