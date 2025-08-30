#ifndef SCENE_H
#define SCENE_H


#include <QList>
#include <QPointF>
#include <QPainter>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QSGNode>
#include <QColor>
// #include "Math/Helper/ClickableMobject.h"
#include "Mobjects/Mobject.h"
#include <QHash>

class ClickableMobject;

class Scene : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ClickableMobject* SelectedMobject READ SelectedMobject NOTIFY SelectedMobjectChanged FINAL)
private:

    int total_mobj;
    int active_m_id=-1;
    int gridsize=50;
    QColor bgcol;
    QHash<int,ClickableMobject*> m_objects;


public:
    Scene() ;
    ~Scene();

    Q_INVOKABLE void add_mobject(QString mobj);
    ClickableMobject* SelectedMobject();;

    void setbg(QColor c){bgcol=c;}
    QColor getbg(){return bgcol;};
    QColor getBorderColor();
    int scalefactor();

    QPointF p2c(QPointF p);
    QPointF c2p(QPointF c);

    void setActiveMobjectId(int val);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;

signals:
    void SelectedMobjectChanged();
};

#endif // SCENE_H
