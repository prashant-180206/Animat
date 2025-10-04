#include "ClickableMobject.h"
#include "Math/Scene.h"

ClickableMobject::ClickableMobject(Scene *canvas, QQuickItem *parent)
    : Mobject(parent)
{
    setFlag(Mobject::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    m_canvas = canvas;

    properties->setBase(new BaseProperties(this->properties)); // REMOVED - prevents double initialization

    connect(properties->base(), &BaseProperties::posChanged, this, [this](const QPointF &newPos)
            { this->setCenter(newPos.x(), newPos.y());});

    connect(properties->base(), &BaseProperties::sizeChanged, this, [this](const QPointF &newSize)
            { this->setSize(newSize.x(), newSize.y()); });

    connect(properties->base(), &BaseProperties::colorChanged, this, [this]
            { update(); });

    connect(properties->base(), &BaseProperties::opacityChanged, this, [this](qreal op)
            {
                this->setOpacity(op);
                update(); });

    connect(properties->base(), &BaseProperties::scaleChanged, this, [this](qreal scale)
            {
                this->setScale(scale);
                update(); });

    connect(properties->base(), &BaseProperties::rotationChanged, this, [this](qreal rotation)
            {
                this->setRotation(rotation);
                update(); });

    connect(properties->base(), &BaseProperties::zindexChanged, this, [this](qreal zindex)
            {
                this->setZ(zindex);
                update(); });

    properties->base()->setColor(m_color);
    properties->base()->setPos(QPoint(0, 0));
    properties->base()->setSize({0, 0});
    properties->base()->setName("Mobject");
    properties->base()->setScale(1.0);
    properties->base()->setRotation(0.0);
    properties->base()->setZindex(0.0);
    properties->base()->setParent(this);
}

void ClickableMobject::setId(QString newid)
{
    m_id = newid;
}



Scene *ClickableMobject::getcanvas() const
{
    return m_canvas;
}

void ClickableMobject::setCenter(qreal xval, qreal yval)
{
    properties->base()->setPos(QPointF(xval, yval));
    QPointF pt = QPointF(xval, yval);

    pt = getcanvas()->p2c(pt);
    setX(pt.x());
    setY(pt.y());

    center = pt;
}

void ClickableMobject::setSize(qreal height, qreal width)
{
    properties->base()->setSize({height, width});
    auto h = height * getcanvas()->scalefactor();
    auto w = width * getcanvas()->scalefactor();
    setHeight(h);
    setWidth(w);
}

MProperties *ClickableMobject::getProperties() const
{
    return properties;
}



QPointF ClickableMobject::getCenter() const
{
    return center;
}

void ClickableMobject::setfromJSON(const QJsonObject &o)
{
    MobjData d = MobjData::fromJSON(o, this);
    setId(d.id);
    if (properties)
        properties->setfromJSON(d.properties.toJson().object());
}

ClickableMobject::MobjData ClickableMobject::getData() const
{
    MobjData d;
    d.id = getId();
    if (this->getProperties()){
        qInfo()<<"PROP AVAIL"<<getId();
        d.properties = getProperties()->getData();
    }
    return d;
}

ClickableMobject::~ClickableMobject()
{
    // Properties are QObject children and will be automatically deleted
    // Don't manually delete to prevent double-delete
    // delete properties;
    // properties = nullptr;
}



void ClickableMobject::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        QPointF pressScenePos = event->scenePosition();
        QPointF pressCanvasPos = m_canvas->mapFromScene(pressScenePos);
        m_dragItemOffset = pressCanvasPos - QPointF(x(), y());
        m_canvas->setActiveId(getId());
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void ClickableMobject::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging)
    {
        QPointF scenePos = event->scenePosition();
        QPointF canvasPos = m_canvas->mapFromScene(scenePos);
        QPointF newCanvasPos = canvasPos - m_dragItemOffset;
        QPointF logicalPos = m_canvas->c2p(newCanvasPos);
        properties->base()->setPos(logicalPos);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void ClickableMobject::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging && event->button() == Qt::LeftButton)
    {
        m_dragging = false;
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

QJsonDocument ClickableMobject::MobjData::toJson() const
{
    QJsonObject o;
    o["id"] = id;
    o["properties"]= properties.toJson().object();

    return QJsonDocument(o);
}

ClickableMobject::MobjData ClickableMobject::MobjData::fromJSON(const QJsonObject &o, const ClickableMobject *parent)
{
    MobjData d;
    d.id = o["id"].toString();
    if (parent && parent->getProperties() && o.contains("properties"))
        d.properties = MProperties::MPropData::fromJSON(o["properties"].toObject(), parent->getProperties());
    return d;
}
