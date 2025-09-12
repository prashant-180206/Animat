#include "ClickableMobject.h"
#include "Math/Scene.h"


ClickableMobject::ClickableMobject(Scene *canvas, QQuickItem *parent)
    : Mobject(parent)
{
    setFlag(Mobject::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    m_canvas = canvas;


    connect(properties, &MProperties::posChanged, this, [this](const QPointF &newPos){
        this->setCenter(newPos.x(), newPos.y());
    });

    connect(properties, &MProperties::sizeChanged, this, [this](const QPointF &newSize){
        this->setSize(newSize.x(), newSize.y());
    });

    connect(properties, &MProperties::colorChanged, this, [this]{
        update();
    });

    connect(properties,&MProperties::opacityChanged,this,[this](qreal op){
        this->setOpacity(op);
        qInfo()<<"OPACITY CHANGED";
        update();
    });

    properties->setColor(m_color);
    properties->setPos(QPoint(0, 0));
    properties->setSize({0, 0});
    properties->setName("Mobject");
    properties->setParent(this);
}

int ClickableMobject::getId() const { return m_id; }

void ClickableMobject::setId(int newid)
{
    m_id=newid;
}

Scene *ClickableMobject::getcanvas() const
{
    return m_canvas;
}

void ClickableMobject::setCenter(qreal xval, qreal yval)
{
    properties->setPos(QPointF(xval,yval));
    QPointF pt = QPointF(xval,yval);

    pt = getcanvas()->p2c(pt);
    setX(pt.x());
    setZ(50);
    setY(pt.y());

    center=pt;
}



void ClickableMobject::setSize(qreal height, qreal width)
{
    properties->setSize({height,width});
    auto h = height *getcanvas()->scalefactor();
    auto w = width *getcanvas()->scalefactor();
    setHeight(h);
    setWidth(w);
}

MProperties* ClickableMobject::getProperties(){return properties;}


QPointF ClickableMobject::getCenter() const
{
    return center;
}

QPointF ClickableMobject::top() const
{
    // Middle of the top edge (x center, y top)
    return QPointF(x() + width() / 2, y());
}

QPointF ClickableMobject::bottom() const
{
    // Middle of the bottom edge (x center, y bottom)
    return QPointF(x() + width() / 2, y() + height());
}

QPointF ClickableMobject::left() const
{
    // Middle of the left edge (x left, y center)
    return QPointF(x(), y() + height() / 2);
}

QPointF ClickableMobject::right() const
{
    // Middle of the right edge (x right, y center)
    return QPointF(x() + width(), y() + height() / 2);
}

ClickableMobject::~ClickableMobject()
{
    delete properties;
    properties = nullptr;
}


void ClickableMobject::mousePressEvent(QMouseEvent *event) {
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

void ClickableMobject::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging) {
        QPointF scenePos = event->scenePosition();
        QPointF canvasPos = m_canvas->mapFromScene(scenePos);
        QPointF newCanvasPos = canvasPos - m_dragItemOffset;
        QPointF logicalPos = m_canvas->c2p(newCanvasPos);
        properties->setPos(logicalPos);
        // setCenter(logicalPos.x(), logicalPos.y());
        event->accept();
    } else {
        event->ignore();
    }
}

void ClickableMobject::mouseReleaseEvent(QMouseEvent *event) {
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    } else {
        event->ignore();
    }
}

