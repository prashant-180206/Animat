#include "Scene.h"
#include "Constants.h"



Scene::Scene()
{
    size=0;
    this->setWidth(DEF_CANVAS_WIDTH);
    this->setHeight(DEF_CANVAS_HEIGHT);

    setFlag(ItemHasContents, true);
    setbg(DEF_CANVAS_BG);

    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow* w){
        if(w) update();
    });
}

Scene::~Scene()
{
    qDeleteAll(m_objects);
    m_objects.clear();
}

void Scene::add_mobject(Mobject *m, QPointF coordinate)
{
    QPointF c = this->position()+ coordinate;
    m->setCenter(c);
    m_objects.append(m);

    m->setId(QString::number(size));
    size++;
}

void Scene::remove_mobject(Mobject *m)
{
    if (m_objects.removeOne(m)) {
        m->setParentItem(nullptr);
        delete m;
    }

}




QPointF Scene::p2c(QPointF p)
{
    double x = p.x() + this->width()/2;
    double y = (p.y() + this->height()/2)*-1;

    return QPointF(x,y);

}

QPointF Scene::c2p(QPointF c) {
    double x = c.x() - this->width() / 2;
    double y = -(c.y()) - this->height() / 2;
    return QPointF(x, y);
}

QSGNode* Scene::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
    QSGNode* rootNode = oldNode;
    if (!rootNode) {
        rootNode = new QSGNode();
    } else {
        rootNode->removeAllChildNodes();
    }

    // Draw background
    QSGSimpleRectNode* bgNode = new QSGSimpleRectNode(boundingRect(), getbg());
    rootNode->appendChildNode(bgNode);



    return rootNode;
}


