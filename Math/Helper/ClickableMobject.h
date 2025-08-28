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
    }

signals:
    void clicked();

protected:


    // Handle mouse click
    void mousePressEvent(QMouseEvent *event) override {
        qDebug() << "ClickableRect clicked at:" << event->pos();
        emit clicked();
        event->accept();
    }
};
