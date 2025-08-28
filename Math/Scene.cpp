#include "Scene.h"

#include <QtGui/qopenglfunctions.h>
// #include "Math/Helper/Text.h"
// #include "Math/Mobjects/Circle.h"
// #include "Math/Mobjects/Curve.h"
// #include "Math/Mobjects/Rectangle.h"
#include "Math/Mobjects/Circle.h"
#include "Math/Mobjects/Curve.h"

#include <Math/Mobjects/line.h>
// #include "Math/Mobjects/ClickableMobject.h"



Scene::Scene()
{
    total_mobj =0;
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

Mobject* Scene::add_mobject(QString mobj)
{
    // auto *m = new Curve(this,this);
    // m->setParentItem(this);

    auto *l = new Line(this,this);
    l->setP1(c2p(QPointF(0,0)));
    l->setP2(c2p(QPointF(200,200)));




    // return rect;



    auto* m= new Curve(this,this);
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


    auto *poly = new Polygon(this, this);
    poly->setParentItem(this);
    poly->setZ(50);
    this->setZ(0);

    QVector<QPointF> points = {
        c2p(QPointF(0, 0)),
        c2p(QPointF(100, 0)),
        c2p(QPointF(100, 100)),
        c2p(QPointF(0, 100)),

    };
    poly->setPoints(points);
    poly->setWidth(200);
    poly->setHeight(200);
    poly->setFillColor(QColor(255, 100, 100, 150));  // semi-transparent red fill
    poly->buildPolygon();
    poly->update();

    auto *circle = new Circle(this, this);
    circle->setParentItem(this);
    circle->setZ(50);
    this->setZ(0);

    circle->setRadius(80);           // Set radius
    circle->setSegmentCount(60);     // Smoothness of approximation
    circle->setFillColor(QColor(100, 150, 255, 150));  // semi-transparent blue fill

    circle->update();

    connect(circle, &ClickableMobject::clicked, this, [=, this]{
        qDebug() << "Scene received Circle click, id:" << total_mobj;
    });

    auto *grp = new Group(this, this);
    grp->setParentItem(this);
    grp->setZ(50);
    grp->setX(200);
    grp->setY(200);
    this->setZ(0);
    grp->setWidth(100);
    grp->setHeight(100);


    // Smoothness of approximation
    // grp->setFillColor(QColor(100, 150, 255, 150));  // semi-transparent blue fill

    grp->update();

    connect(grp, &ClickableMobject::clicked, this, [=, this]{
        qDebug() << "Scene received Circle click, id:" << total_mobj;
    });

    return grp;

    // auto *rect = new Rectangle(this, this);
    // rect->setParentItem(this);
    // rect->setZ(50);
    // this->setZ(0);

    // rect->setRectWidth(200);
    // rect->setRectHeight(120);
    // rect->setFillColor(QColor(150, 255, 150, 150));  // semi-transparent green fill

    // rect->update();

    // // Create a new TextMobject instance as child of current item
    // Text *label = new Text(this,this);
    // label->setText("Hello QtQuick! \n qwertyuiop");
    // label->setColor(Qt::white);
    // label->setFontSize(12);
    // label->setPosition(c2p(QPointF(0,0)));

    // // Request update if needed
    // label->update();


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



