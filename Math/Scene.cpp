#include "Scene.h"

#include <QtGui/qopenglfunctions.h>
#include "Math/Mobjects/Circle.h"
#include "Math/Mobjects/Curve.h"
#include "Math/Mobjects/Rectangle.h"



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

}

Mobject* Scene::add_mobject(QString mobj)
{
    auto *m = new Curve(this,this);
    m->setParentItem(this);

    m->setZ(50);
    this->setZ(0);

    // auto *m1 = new Line(this,this);
    // m1->setP1(c2p(QPointF(0,0)));
    // m1->setP2(c2p(QPointF(100,100)));
    // m1->update();
    // m1->setZ(40);
    // // m1->setHeight(100);
    // // m1->setWidth(100);
    // m1->setParentItem(this);



    m->setParameterRange(-100, 100);
    m->setSegmentCount(5);

    // Override curve function with scaling to widget size
    m->setCurveFunction([=, this](double t) {
        // Map parametric coordinates to item space (scaled and inverted Y if needed)
        double x = t;
        double y = t*t*0.01; // y = t^2 inverted Y
        return c2p(QPointF(x, y));
    });

    m->update();


    // auto *poly = new Polygon(this, this);
    // poly->setParentItem(this);
    // poly->setZ(50);
    // this->setZ(0);

    // QVector<QPointF> points = {
    //     c2p(QPointF(0, 0)),
    //     c2p(QPointF(100, 0)),
    //     c2p(QPointF(100, 100)),
    //     c2p(QPointF(0, 100)),

    // };
    // poly->setPoints(points);
    // poly->setFillColor(QColor(255, 100, 100, 150));  // semi-transparent red fill
    // poly->buildPolygon();
    // poly->update();

    auto *circle = new Circle(this, this);
    circle->setParentItem(this);
    circle->setZ(50);
    this->setZ(0);

    circle->setRadius(80);           // Set radius
    circle->setSegmentCount(60);     // Smoothness of approximation
    circle->setFillColor(QColor(100, 150, 255, 150));  // semi-transparent blue fill

    circle->update();

    auto *rect = new Rectangle(this, this);
    rect->setParentItem(this);
    rect->setZ(50);
    this->setZ(0);

    rect->setRectWidth(200);
    rect->setRectHeight(120);
    rect->setFillColor(QColor(150, 255, 150, 150));  // semi-transparent green fill

    rect->update();


    return m;
}





QPointF Scene::p2c(QPointF p)
{
    double x = p.x() + this->width()/2;
    double y = (p.y() + this->height()/2)*-1;

    return QPointF(x,y);

}

QPointF Scene::c2p(QPointF c) {
    double x = c.x()+this->width()/2;
    double y = -c.y() +this->height()/2;
    return QPointF(x, y);
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



