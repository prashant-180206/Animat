#pragma once
#include "Math/scene.h"
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QMouseEvent>
#include <QDebug>

class ClickableMobject : public Mobject {
    Q_OBJECT
public:
    explicit ClickableMobject(Scene * canvas,QQuickItem *parent = nullptr);

signals:
    void clicked();

private:
    // Scene* m_canvas;
protected:

    void mousePressEvent(QMouseEvent *event) override;
};
