#ifndef GROUP_H
#define GROUP_H

#include <QQuickItem>
#include "Mobject.h"

class Group : public Mobject
{
    Q_OBJECT
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)

public:
    explicit Group(Scene* canvas=nullptr,QQuickItem *parent = nullptr);

    int spacing() const;
    void setSpacing(int spacing);

    Q_INVOKABLE void arrange();
    Q_INVOKABLE void addMember(Mobject *item);
    Q_INVOKABLE void removeMember(Mobject *item);

signals:
    void spacingChanged();

private:
    int m_spacing;
};

#endif // GROUP_H
