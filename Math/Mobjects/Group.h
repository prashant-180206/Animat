#ifndef GROUP_H
#define GROUP_H

#include <QQuickItem>
#include "Math/Helper/ClickableMobject.h"
#include "Mobject.h"

class Group : public ClickableMobject
{
    Q_OBJECT
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)

public:
    explicit Group(Scene* canvas,QQuickItem *parent = nullptr);

    int spacing() const;
    void setSpacing(int spacing);

    void arrange();
    void addMember(Mobject *item);
    void removeMember(Mobject *item);

signals:
    void spacingChanged();

private:
    int m_spacing;
};

#endif // GROUP_H
