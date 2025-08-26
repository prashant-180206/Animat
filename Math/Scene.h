#ifndef SCENE_H
#define SCENE_H


#include <QList>
#include <QPointF>
#include <QPainter>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QSGNode>
#include <QColor>
#include "Constants.h"
#include "Mobjects/Mobject.h"

class Scene : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    Scene() ;
    ~Scene();

    Q_INVOKABLE Mobject* add_mobject(QString mobj);
    // void remove_mobject(Mobject* m);

    void setbg(QColor c){bgcol=c;}
    QColor getbg(){return bgcol;};
    QColor getBorderColor(){return TEXT_LIGHT;};

    QPointF p2c(QPointF p);
    QPointF c2p(QPointF c);
private:
    // QList<Mobject*> m_objects;
    int size;
    QColor bgcol;


protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;


signals:
};

#endif // SCENE_H
