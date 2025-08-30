#ifndef GROUP_H
#define GROUP_H

#include <QQuickItem>
#include <QRectF>
#include "Math/Helper/ClickableMobject.h"
#include "Mobject.h"

class Group : public ClickableMobject
{
    Q_OBJECT
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)

public:
    explicit Group(Scene* canvas, QQuickItem *parent = nullptr);
    ~Group();  // Destructor to manage children destruction

    int spacing() const;
    void setSpacing(int spacing);

    void arrange();
    void addMember(Mobject *item);
    void removeMember(Mobject *item);
    // virtual void setCenter(qreal xval, qreal yval) override;

signals:
    void spacingChanged();

private:
    int m_spacing;
    QRectF m_membersBoundingRect;
};

#endif // GROUP_H
