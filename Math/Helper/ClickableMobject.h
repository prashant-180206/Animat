#pragma once
#include "Math/scene.h"
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QMouseEvent>
#include <QDebug>

class ClickableMobject : public Mobject {
    Q_OBJECT
public:
    explicit ClickableMobject(Scene * canvas,QQuickItem *parent = nullptr)
        : Mobject(canvas,parent)
    {
        setFlag(Mobject::ItemHasContents, true);   // tells Qt we will draw
        setAcceptedMouseButtons(Qt::AllButtons);      // enable mouse input
        qDebug() << "ClickableRect created";
        m_canvas = canvas;

    }

signals:
    void clicked();

private:
    Scene* m_canvas;
protected:

    void mousePressEvent(QMouseEvent *event) override {
        m_canvas->setActiveMobject(getId());
        qDebug()<<"Mobject "<<getId()<<" clicked";
        emit clicked();
        event->accept();
    }
};
