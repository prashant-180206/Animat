#ifndef MOBJECT_H
#define MOBJECT_H

#include <QQuickItem>
#include <QPointF>

class Scene;

class Mobject : public QQuickItem
{
    Q_OBJECT
public:
    explicit Mobject(Scene* canvas=nullptr,QQuickItem *parent = nullptr);

    QPointF getCenter() const;
    QPointF top() const;
    QPointF bottom() const;
    QPointF left() const;
    QPointF right() const;
};

#endif // MOBJECT_H
