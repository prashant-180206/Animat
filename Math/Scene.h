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
#include <QHash>

class Scene : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int activeMobject READ getActiveMobject WRITE setActiveMobject NOTIFY activeMobjectChanged)
private:

    int total_mobj;
    int active_m_id=-1;
    QColor bgcol;
    QHash<int,Mobject*> m_objects;


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

    int getActiveMobject() const { return active_m_id; }
    void setActiveMobject(int val) {
        if (active_m_id != val) {
            active_m_id = val;
            emit activeMobjectChanged(active_m_id);
        }
    }

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;


signals:
    void activeMobjectChanged(int newId);
};

#endif // SCENE_H
