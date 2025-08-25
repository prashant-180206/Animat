#ifndef SCENE_H
#define SCENE_H

#include "Mobjects/Mobject.h"
#include <QList>
#include <QPointF>
#include <QPainter>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QSGNode>
#include <QColor>

#include "Constants.h"

class Scene : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    Scene() ;
    ~Scene();

    void add_mobject(Mobject* m,QPointF coordinate);
    void remove_mobject(Mobject* m);

    void setbg(QColor c){bgcol=c;}
    QColor getbg(){return bgcol;};
    QColor getBorderColor(){return TEXT_LIGHT;};

    QPointF p2c(QPointF p);
    QPointF c2p(QPointF c);
private:
    QList<Mobject*> m_objects;
    int size;
    QColor bgcol;


protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;


signals:
};

#endif // SCENE_H
