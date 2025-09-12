#ifndef SCENE_H
#define SCENE_H


#include <QList>
#include <QPointF>
#include <QPainter>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QSGNode>
#include <QColor>
#include "Mobjects/Mobject.h"
#include "animationmanager.h"
#include "mproperties.h"
#include "playbackslider.h"
#include "trackermanager.h"
#include <QHash>

class ClickableMobject;

class Scene : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ClickableMobject* SelectedMobject READ SelectedMobject NOTIFY SelectedMobjectChanged FINAL)


public:
    Scene() ;
    ~Scene();

    Q_INVOKABLE void add_mobject(QString mobj);

    ClickableMobject* SelectedMobject();

    Q_INVOKABLE TrackerManager* trackers();

    Q_INVOKABLE PlaybackSlider * player();

    Q_INVOKABLE AnimationManager* animator();;

    Q_INVOKABLE MProperties * getProperties(){return m_prop;};

    ClickableMobject * getMobject(int id){
        return m_objects[id];
    }

    void setbg(QColor c){bgcol=c;}
    QColor getbg(){return bgcol;};
    QColor getBorderColor();

    int scalefactor();

    QPointF p2c(QPointF p);
    QPointF c2p(QPointF c);

    void setActiveMobjectId(int val);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;

private:

    int total_mobj;
    int active_m_id=-1;
    int gridsize=50;
    QColor bgcol;
    QHash<int,ClickableMobject*> m_objects;
    MProperties* m_prop = new MProperties(this);

    TrackerManager* m_trackers= new TrackerManager(this);
    PlaybackSlider* m_player = new PlaybackSlider(this);
    AnimationManager * m_animator = new AnimationManager(this);


signals:
    void SelectedMobjectChanged();
};

#endif // SCENE_H
