#ifndef MOBJECT_H
#define MOBJECT_H

#include <QQuickItem>
#include <QPointF>

class Scene;

class Mobject : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged FINAL)

private:
    int m_id = 0;
    Scene* m_canvas = nullptr;

public:
    explicit Mobject(Scene* canvas, QQuickItem *parent = nullptr);


    int getId() const { return m_id; }
    void setId(int newid) {
        if (m_id != newid) {
            m_id = newid;
            emit idChanged();
        }
    }

    // Q_INVOKABLE void initialize();


    QPointF getCenter() const;
    QPointF top() const;
    QPointF bottom() const;
    QPointF left() const;
    QPointF right() const;

signals:
    void idChanged();

};

#endif // MOBJECT_H
