#pragma once
#include "Math/Helper/mproperties.h"
#include "Math/Mobjects/Mobject.h"
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QMouseEvent>
#include <QDebug>
#include <QVariantMap>


class ClickableMobject : public Mobject {

    Q_OBJECT

public:
    explicit ClickableMobject(Scene * canvas,QQuickItem *parent = nullptr);

    QString getId() const { return m_id; }
    void setId(QString newid)
    {
        m_id=newid;
    }
    Scene* getcanvas() const;

    virtual void setCenter(qreal xval, qreal yval);
    void setSize( qreal height,qreal width);

    Q_INVOKABLE MProperties* getProperties();;


    QPointF getCenter() const;
    QPointF top() const;
    QPointF bottom() const;
    QPointF left() const;
    QPointF right() const;

    virtual ~ClickableMobject();

private:

    QColor m_color = Qt::blue;

    Scene* m_canvas = nullptr;
    QPointF center = QPointF(0,0);

    bool m_dragging = false;
    QPointF m_dragItemOffset;
    QPointF m_itemStartPos;
    QPointF m_localpos;

protected:
    QString m_id = 0;
    MProperties* properties = new MProperties(this);


    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

};
