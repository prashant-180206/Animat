#include "Scene.h"

#include <QtGui/qopenglfunctions.h>
#include "Utils/Constants.h"

#include <Math/Mobjects/line.h>
#include "Utils/mobjectmap.h"


Scene::Scene()
{
    total_mobj =0;
    setWidth(DEF_CANVAS_WIDTH);
    setHeight(DEF_CANVAS_HEIGHT);

    qDebug()<<height()<<width();

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
    // m->getProperties()->setOpacity(0.1);
    m->setCenter(5,4);
    m->setZ(20);
    qDebug()<<m<<m->getCenter();
    m_objects.insert(total_mobj,m);

    total_mobj++;

}

ClickableMobject *Scene::SelectedMobject(){return active_m_id>=0?m_objects[active_m_id]:nullptr;}

TrackerManager *Scene::trackers(){
    return m_trackers;
}

PlaybackSlider *Scene::player(){
    return m_player;
}

AnimationManager *Scene::animator(){
    return m_animator;
}


QColor Scene::getBorderColor(){return TEXT_LIGHT;}

int Scene::scalefactor(){return gridsize;}

QPointF Scene::p2c(QPointF p) {
    double x = p.x() * gridsize ;
    double y = p.y() * gridsize ;
    auto res =QPointF(x, y);
    res = mapToItem(this,res);
    return res;
}

QPointF Scene::c2p(QPointF c) {
    c = mapToItem(this,c);
    double x = (c.x() ) / gridsize;
    double y = (c.y() ) / gridsize;
    auto res=QPointF(x, y);
    return res;
}

void Scene::setActiveMobjectId(int val) {
    if (active_m_id != val) {
        active_m_id = val;
    }
    emit SelectedMobjectChanged();
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



