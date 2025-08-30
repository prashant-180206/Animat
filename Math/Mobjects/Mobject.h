#ifndef MOBJECT_H
#define MOBJECT_H

#include <QQuickItem>

class Scene;

class Mobject : public QQuickItem
{
    Q_OBJECT

private:

public:
    explicit Mobject(QQuickItem *parent = nullptr);

};

#endif // MOBJECT_H
