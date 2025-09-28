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
    Q_PROPERTY(ClickableMobject *SelectedMobject READ SelectedMobject NOTIFY SelectedMobjectChanged FINAL)
    Q_PROPERTY(QString activeId READ activeId WRITE setActiveId NOTIFY activeIdChanged FINAL)
    Q_PROPERTY(QStringList mobjectIds READ getAllMobjectIds NOTIFY mobjectsChanged FINAL)

public:
    Scene();
    ~Scene();

    QString activeId() { return active_m_id; };

    Q_INVOKABLE void add_mobject(QString mobj);

    Q_INVOKABLE void removeMobject(QString mobjectId);

    ClickableMobject *SelectedMobject();

    Q_INVOKABLE TrackerManager *trackers();

    Q_INVOKABLE PlaybackSlider *player();
    Q_INVOKABLE AnimationManager *animator();
    ;
    Q_INVOKABLE MProperties *getProperties() { return m_prop; };

    Q_INVOKABLE ClickableMobject *getMobject(QString id);

    Q_INVOKABLE QStringList getAllMobjectIds();

    void setbg(QColor c) { bgcol = c; }
    QColor getbg() { return bgcol; };
    QColor getBorderColor();

    int scalefactor();

    QPointF p2c(QPointF p);
    QPointF c2p(QPointF c);

    void setActiveId(QString val)
    {
        if (active_m_id != val)
        {
            active_m_id = val;
        }
        emit SelectedMobjectChanged();
        emit activeIdChanged();
    }

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    int total_mobj;
    QString active_m_id = "";
    int gridsize = 50;
    QColor bgcol;
    QHash<QString, ClickableMobject *> m_objects;
    MProperties *m_prop = new MProperties(this);

    TrackerManager *m_trackers = new TrackerManager(this, this);
    PlaybackSlider *m_player = new PlaybackSlider(this);
    AnimationManager *m_animator = new AnimationManager(this);

signals:
    void SelectedMobjectChanged();
    void activeIdChanged();
    void mobjectsChanged();
};

#endif // SCENE_H
