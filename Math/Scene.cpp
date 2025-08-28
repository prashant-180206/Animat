#include "Scene.h"

#include <QtGui/qopenglfunctions.h>
#include "Constants.h"

#include <Math/Mobjects/line.h>
#include "Utils/mobjectmap.h"


Scene::Scene()
{
    total_mobj =0;
    this->setWidth(DEF_CANVAS_WIDTH);
    this->setHeight(DEF_CANVAS_HEIGHT);

    setFlag(ItemHasContents, true);
    setbg(DEF_CANVAS_BG);
    setZ(0);
    MobjectMap::init(this);

    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow* w){
        if(w) update();
    });
}

Scene::~Scene()
{
    qDeleteAll(m_objects);
    m_objects.clear();

}

void Scene::add_mobject(QString mobj)
{

    auto *m = MobjectMap::map[mobj]();
    qDebug()<<"Adding Mobject";
    if (!m) return;

    m->setParentItem(this);
    m->setId(total_mobj);
    qDebug()<<""<<m->getCenter();
    // m->setHeight(200);
    m->setCenter(0,0); //sending c to set p
    qDebug()<<""<<m->getCenter();
    qDebug()<<""<<m->height()<<m->width();
    m->setZ(2);
    // m->setY(200-m->height());
    // m->setX(200-m->width());
    qDebug()<<m;

    // auto *l= new Line(this,this);
    // l->setP1(QPointF(0,0));
    // l->setP2(QPointF(100,100));

    total_mobj++;

}

QColor Scene::getBorderColor(){return TEXT_LIGHT;}

int Scene::scalefactor(){return gridsize;}


QPointF Scene::p2c(QPointF p)
{
    double x = p.x()*gridsize - width()/2;
    double y = -(p.y()*gridsize + height()/2);
    return QPointF(x,y);

}

QPointF Scene::c2p(QPointF c) {
    double x =(c.x()*gridsize+width()/2);
    double y =(-c.y()*gridsize+height()/2);
    return QPointF(x, y);
}

int Scene::getActiveMobject() const { return active_m_id; }

void Scene::setActiveMobject(int val) {
    if (active_m_id != val) {
        active_m_id = val;
        emit activeMobjectChanged(active_m_id);
    }
}

QSGNode* Scene::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    QSGNode* rootNode = oldNode;
    if (!rootNode) {
        rootNode = new QSGNode();
    } else {
        rootNode->removeAllChildNodes();
    }

    QRectF rect = boundingRect();

    // Draw background
    QSGSimpleRectNode* bgNode = new QSGSimpleRectNode(rect, getbg());
    rootNode->appendChildNode(bgNode);

    // Draw border
    QSGGeometryNode* borderNode = new QSGGeometryNode();
    QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 5);
    geometry->setDrawingMode(GL_LINE_STRIP);
    borderNode->setGeometry(geometry);
    borderNode->setFlag(QSGNode::OwnsGeometry);

    QSGGeometry::Point2D* vertices = geometry->vertexDataAsPoint2D();
    vertices[0].set(rect.left(), rect.top());
    vertices[1].set(rect.right(), rect.top());
    vertices[2].set(rect.right(), rect.bottom());
    vertices[3].set(rect.left(), rect.bottom());
    vertices[4].set(rect.left(), rect.top());  // Close loop

    auto* material = new QSGFlatColorMaterial();
    material->setColor(getBorderColor());
    borderNode->setMaterial(material);
    borderNode->setFlag(QSGNode::OwnsMaterial);

    borderNode->setFlag(QSGNode::OwnsMaterial);

    rootNode->appendChildNode(borderNode);

    return rootNode;
}



