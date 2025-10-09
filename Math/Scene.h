#ifndef SCENE_H
#define SCENE_H

#include <QList>
#include <QPointF>
#include <QPainter>
#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QSGNode>
#include <QColor>
#include "Mobjects/Base/Mobject.h"
#include "Animations/animationmanager.h"
#include "Helper/mproperties.h"
#include "ValueTracker/playbackslider.h"
#include "Parser/trackermanager.h"
#include <QHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class ClickableMobject;
class Parser;

class Scene : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ClickableMobject *SelectedMobject READ SelectedMobject NOTIFY SelectedMobjectChanged FINAL)
    Q_PROPERTY(QString activeId READ activeId WRITE setActiveId NOTIFY activeIdChanged FINAL)
    Q_PROPERTY(QStringList mobjectIds READ getAllMobjectIds NOTIFY mobjectsChanged FINAL)
    Q_PROPERTY(bool showBorders READ showBorders WRITE setShowBorders NOTIFY showBordersChanged FINAL)

public:
    Scene();
    ~Scene();

    QString activeId() { return active_m_id; };

    Q_INVOKABLE void add_mobject(QString mobj, QString name = "");
    Q_INVOKABLE Parser *getParser();

    Q_INVOKABLE void removeMobject(QString mobjectId);

    ClickableMobject *SelectedMobject();

    Q_INVOKABLE PlaybackSlider *player();
    Q_INVOKABLE AnimationManager *animator() const { return m_animator; };
    Q_INVOKABLE MProperties *getProperties() { return m_prop; };

    Q_INVOKABLE ClickableMobject *getMobject(QString id);

    Q_INVOKABLE QStringList getAllMobjectIds();

    void setbg(QColor c) { bgcol = c; }
    QColor getbg() { return bgcol; };
    QColor getBorderColor();

    // Border visibility control
    bool showBorders() const { return m_showBorders; }
    void setShowBorders(bool show);

    struct SceneData
    {
        QString activeId;
        int gridSize = 50;
        QColor backgroundColor;
        bool showBorders = true;
        QJsonObject animatorData;
        QJsonObject playerData;
        QJsonObject parserData;
        QJsonObject trackerData;
        QJsonArray mobjectsData;

        QJsonDocument toJson() const;

        static SceneData fromJSON(const QJsonObject &o);
        ;
    };

    SceneData getData() const;
    ;

    void setFromJSON(const QJsonObject &o);

    int scalefactor();

    QPointF p2c(QPointF p);
    QPointF c2p(QPointF c);

    void setActiveId(QString val);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    int total_mobj;
    QString active_m_id = "";
    int gridsize = 70;
    QColor bgcol;
    QHash<QString, ClickableMobject *> m_objects;
    MProperties *m_prop = new MProperties(this);
    PlaybackSlider *m_player = new PlaybackSlider(this);
    AnimationManager *m_animator = new AnimationManager(this);
    bool m_showBorders = true; // Default to showing borders

    Parser *m_parser = nullptr;

signals:
    void SelectedMobjectChanged();
    void activeIdChanged();
    void mobjectsChanged();
    void showBordersChanged();
};

#endif // SCENE_H
