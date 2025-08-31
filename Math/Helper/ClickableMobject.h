#pragma once
#include "Math/scene.h"
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QMouseEvent>
#include <QDebug>
#include <QVariantMap>


class ClickableMobject : public Mobject {

    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)

public:
    explicit ClickableMobject(Scene * canvas,QQuickItem *parent = nullptr);

    int getId() const;
    void setId(int newid);
    Scene* getcanvas();

    virtual void setCenter(qreal xval, qreal yval);
    void setSize( qreal height,qreal width);

    Q_INVOKABLE QVariantMap getProperties();;

    QColor color(){return m_color;};
    void setColor(QColor c){
        m_color= c;
        emit colorChanged();
    }

    QPointF getCenter() const;
    QPointF top() const;
    QPointF bottom() const;
    QPointF left() const;
    QPointF right() const;

    virtual ~ClickableMobject() = default;

signals:
 void   colorChanged();

private:

    QColor m_color = Qt::blue;

    Scene* m_canvas = nullptr;
    QPointF center = QPointF(0,0);

    bool m_dragging = false;
    QPointF m_dragItemOffset;
    QPointF m_itemStartPos;
    QPointF m_localpos;

protected:
    int m_id = 0;

    QVariantMap properties;


    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_dragging = true;
            QPointF pressScenePos = event->scenePosition();
            QPointF pressCanvasPos = m_canvas->mapFromScene(pressScenePos);
            m_dragItemOffset = pressCanvasPos - QPointF(x(), y());
            m_canvas->setActiveMobjectId(getId());
            event->accept();
        } else {
            event->ignore();
        }
    }



    void mouseMoveEvent(QMouseEvent *event) override {
        if (m_dragging) {
            QPointF scenePos = event->scenePosition();
            QPointF canvasPos = m_canvas->mapFromScene(scenePos);
            QPointF newCanvasPos = canvasPos - m_dragItemOffset;
            QPointF logicalPos = m_canvas->c2p(newCanvasPos);
            setCenter(logicalPos.x(), logicalPos.y());
            event->accept();
        } else {
            event->ignore();
        }
    }


    void mouseReleaseEvent(QMouseEvent *event) override {
        if (m_dragging && event->button() == Qt::LeftButton) {
            // auto p =getcanvas()->c2p(event->scenePosition());
            // setCenter(p.x(),p.y());
            m_dragging = false;
            event->accept();
        } else {
            event->ignore();
        }
    }

};
