#include "Scene.h"

#include <QtGui/qopenglfunctions.h>
#include "Utils/Constants.h"

#include <Math/Mobjects/Basic/line.h>
#include "Utils/mobjectmap.h"
#include "Parser/trackermanager.h"
#include "Parser/parser.h"

Scene::Scene()
{
    total_mobj = 0;
    setWidth(DEF_CANVAS_WIDTH);
    setHeight(DEF_CANVAS_HEIGHT);

    qDebug() << height() << width();

    setFlag(ItemHasContents, true);
    setbg(DEF_CANVAS_BG);
    setZ(0);
    MobjectMap::init(this);
    setScale(0.8);

    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow *w)
            {
        if(w) update(); });

    // Connect player to animator for time updates
    connect(m_player, &ValueTracker::valueChanged, m_animator, [this](qreal v)
            {
        if(animator()->activePacket()){
            animator()->setTime(v/1000);
        } });

    // Connect the PlaybackSlider to the AnimationManager for dynamic duration
    m_player->setAnimationManager(m_animator);
}

Scene::~Scene()
{
    qDeleteAll(m_objects);
    m_objects.clear();

    delete m_parser;
    m_parser = nullptr;
}
Parser *Scene::getParser()
{
    if (!m_parser)
    {
        m_parser = new Parser(this, this);
    }
    return m_parser;
}

void Scene::add_mobject(QString mobj, QString name)
{

    auto *m = MobjectMap::map[mobj]();
    qDebug() << "Adding Mobject";
    if (!m)
        return;

    auto mbj_id = QString("%1%2").arg(m->getProperties()->base()->name()).arg(total_mobj);
    if (name != "")
    {
        mbj_id = name;
    }

    m->setParentItem(this);
    m->setId(mbj_id);

    m->setCenter(0, 0);
    m->getProperties()->base()->setZindex(z() + 0.1 * total_mobj + 0.1);
    qDebug() << m << m->getCenter();
    m_objects.insert(mbj_id, m);

    total_mobj++;
    emit mobjectsChanged();
}

void Scene::removeMobject(QString mobjectId)
{
    if (m_objects.contains(mobjectId))
    {
        ClickableMobject *mobj = m_objects.take(mobjectId);
        if (mobj)
        {
            mobj->deleteLater();
        }

        // Clear active selection if we're deleting the selected mobject
        if (active_m_id == mobjectId)
        {
            setActiveId("");
        }

        emit mobjectsChanged();
        qDebug() << "Removed mobject:" << mobjectId;
    }
}

ClickableMobject *Scene::SelectedMobject() { return active_m_id != "" ? m_objects[active_m_id] : nullptr; }

QStringList Scene::getAllMobjectIds()
{
    return m_objects.keys();
}

ClickableMobject *Scene::getMobject(QString id)
{
    return m_objects.value(id, nullptr);
}

PlaybackSlider *Scene::player()
{
    return m_player;
}

QColor Scene::getBorderColor() { return TEXT_LIGHT; }

void Scene::setShowBorders(bool show)
{
    if (m_showBorders != show)
    {
        m_showBorders = show;
        update(); // Trigger repaint
        emit showBordersChanged();
    }
}

void Scene::setFromJSON(const QJsonObject &o)
{
    qInfo() << "SCENE DATA SCENE CALLED ";
    SceneData data = SceneData::fromJSON(o);

    // Set basic scene properties
    setActiveId(data.activeId);
    gridsize = data.gridSize;
    setbg(data.backgroundColor);
    setShowBorders(data.showBorders);

    // Clear existing mobjects
    qDeleteAll(m_objects);
    m_objects.clear();

    // Recreate mobjects from data
    for (const QJsonValue &mobjectVal : std::as_const(data.mobjectsData))
    {
        if (mobjectVal.isObject())
        {
            QJsonObject mobjectData = mobjectVal.toObject();
            QString id = mobjectData["id"].toString();
            QString type = mobjectData["properties"].toObject()["base"].toObject()["type"].toString();

            this->add_mobject(type, id);
            qInfo() << "ADDING MOBJECTS";
            getMobject(id)->setfromJSON(mobjectData);
        }
    }

    // Restore managers state
    if (m_animator && !data.animatorData.isEmpty())
    {
        m_animator->setFromJSON(data.animatorData, this);
        qInfo() << "ANIMDATA AVAIL ";
    }
    if (m_player && !data.playerData.isEmpty())
    {
        m_player->setFromJSON(data.playerData);
    }
    // if (m_trackers && !data.trackerData.isEmpty())
    // {
    //     m_trackers->setFromJSON(data.trackerData);
    // }
}

Scene::SceneData Scene::getData() const
{
    SceneData data;
    data.activeId = active_m_id;
    data.gridSize = gridsize;
    data.backgroundColor = bgcol;
    data.showBorders = m_showBorders;

    // Get data from all managers
    if (m_animator)
    {
        data.animatorData = animator()->getData().toJson().object();
    }
    if (m_player)
    {
        data.playerData = m_player->getData().toJson().object();
    }
    // if (m_parser)
    // {
    //     // Note: Parser getData() method needs to be implemented
    //     data.parserData = QJsonObject(); // Placeholder
    // }
    // if (m_trackers)
    // {
    //     data.trackerData = m_trackers->getData().toJson().object();
    // }

    // Get all mobjects data
    QJsonArray mobjectsArray;
    for (auto m : m_objects)
    {
        qInfo() << "lOOPING OVER MOBJECTS";
        mobjectsArray.append(m->getData().toJson().object());
    }

    data.mobjectsData = mobjectsArray;

    return data;
}

int Scene::scalefactor() { return gridsize; }

QPointF Scene::p2c(QPointF p)
{
    double x = p.x() * gridsize + width() / 4;
    double y = -p.y() * gridsize + height() / 4;
    auto res = QPointF(x, y);
    res = mapFromItem(this, res);
    return res;
}

QPointF Scene::c2p(QPointF c)
{
    c = mapFromItem(this, c);
    double x = (c.x() - width() / 4) / gridsize;
    double y = (-c.y() + height() / 4) / gridsize;
    auto res = QPointF(x, y);
    return res;
}

void Scene::setActiveId(QString val)
{
    if (active_m_id != val)
    {
        active_m_id = val;
    }
    emit SelectedMobjectChanged();
    emit activeIdChanged();
}

QSGNode *Scene::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGNode *rootNode = oldNode;
    if (!rootNode)
    {
        rootNode = new QSGNode();
    }
    else
    {
        rootNode->removeAllChildNodes();
    }

    QRectF rect = boundingRect();
    // Inset border by 0.5 pixels to avoid clipping
    QRectF borderRect = rect.adjusted(0.5, 0.5, -0.5, -0.5);

    // Draw background
    QSGSimpleRectNode *bgNode = new QSGSimpleRectNode(rect, getbg());
    rootNode->appendChildNode(bgNode);

    // Draw border only if showBorders is true
    if (m_showBorders)
    {
        QSGGeometryNode *borderNode = new QSGGeometryNode();
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 5);
        geometry->setDrawingMode(GL_LINE_STRIP);
        borderNode->setGeometry(geometry);
        borderNode->setFlag(QSGNode::OwnsGeometry);

        QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
        vertices[0].set(borderRect.left(), borderRect.top());
        vertices[1].set(borderRect.right(), borderRect.top());
        vertices[2].set(borderRect.right(), borderRect.bottom());
        vertices[3].set(borderRect.left(), borderRect.bottom());
        vertices[4].set(borderRect.left(), borderRect.top()); // Close loop

        auto *material = new QSGFlatColorMaterial();
        material->setColor(getBorderColor());
        borderNode->setMaterial(material);
        borderNode->setFlag(QSGNode::OwnsMaterial);

        rootNode->appendChildNode(borderNode);
    }

    return rootNode;
}

// Tracker command execution methods

QJsonDocument Scene::SceneData::toJson() const
{
    QJsonObject o;
    o["activeId"] = activeId;
    o["gridSize"] = gridSize;
    o["backgroundColor"] = QJsonObject{{"r", backgroundColor.red()}, {"g", backgroundColor.green()}, {"b", backgroundColor.blue()}, {"a", backgroundColor.alpha()}};
    o["showBorders"] = showBorders;
    o["animator"] = animatorData;
    o["player"] = playerData;
    o["parser"] = parserData;
    o["tracker"] = trackerData;
    o["mobjects"] = mobjectsData;
    return QJsonDocument(o);
}

Scene::SceneData Scene::SceneData::fromJSON(const QJsonObject &o)
{
    SceneData d;
    d.activeId = o["activeId"].toString();
    d.gridSize = o["gridSize"].toInt();
    auto bg = o["backgroundColor"].toObject();
    d.backgroundColor = QColor(bg["r"].toInt(), bg["g"].toInt(), bg["b"].toInt(), bg["a"].toInt());
    d.showBorders = o["showBorders"].toBool();
    d.animatorData = o["animator"].toObject();
    d.playerData = o["player"].toObject();
    d.parserData = o["parser"].toObject();
    d.trackerData = o["tracker"].toObject();
    d.mobjectsData = o["mobjects"].toArray();
    return d;
}
