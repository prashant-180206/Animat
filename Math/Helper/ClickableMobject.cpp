#include "ClickableMobject.h"


ClickableMobject::ClickableMobject(Scene *canvas, QQuickItem *parent)
    : Mobject(canvas,parent)
{
    setFlag(Mobject::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
}

void ClickableMobject::mousePressEvent(QMouseEvent *event) {
    getcanvas()->setActiveMobject(getId());
    qDebug()<<"Mobject "<<getId()<<" clicked";
    emit clicked();
    event->accept();
}
