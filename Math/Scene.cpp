#include "Scene.h"

#include <QtGui/qopenglfunctions.h>
#include "Utils/Constants.h"

#include <Math/Mobjects/Basic/line.h>
#include "Utils/mobjectmap.h"
#include "Parser/trackermanager.h"

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

    m->setCenter(5, 4);
    m->setZ(total_mobj * 0.1);
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

Parser *Scene::parser()
{
    return m_parser;
}

TrackerManager *Scene::trackers()
{
    return m_trackers;
}

PlaybackSlider *Scene::player()
{
    return m_player;
}

AnimationManager *Scene::animator()
{
    return m_animator;
}

double Scene::evaluate(const QString &expression)
{
    return m_parser->evaluate(expression);
}

void Scene::setParserVariable(const QString &name, double value)
{
    m_parser->setVariable(name, value);
}

double Scene::getParserVariable(const QString &name)
{
    return m_parser->getVariable(name);
}

void Scene::clearParserVariables()
{
    m_parser->clearVariables();
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

int Scene::scalefactor() { return gridsize; }

QPointF Scene::p2c(QPointF p)
{
    double x = p.x() * gridsize;
    double y = p.y() * gridsize;
    auto res = QPointF(x, y);
    res = mapToItem(this, res);
    return res;
}

QPointF Scene::c2p(QPointF c)
{
    c = mapToItem(this, c);
    double x = (c.x()) / gridsize;
    double y = (c.y()) / gridsize;
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
        vertices[0].set(rect.left(), rect.top());
        vertices[1].set(rect.right(), rect.top());
        vertices[2].set(rect.right(), rect.bottom());
        vertices[3].set(rect.left(), rect.bottom());
        vertices[4].set(rect.left(), rect.top()); // Close loop

        auto *material = new QSGFlatColorMaterial();
        material->setColor(getBorderColor());
        borderNode->setMaterial(material);
        borderNode->setFlag(QSGNode::OwnsMaterial);

        rootNode->appendChildNode(borderNode);
    }

    return rootNode;
}

// Tracker command execution methods
void Scene::executeTrackerScript(const QString &script)
{
    m_trackers->parseScript(script);
}

QStringList Scene::getTrackerNames()
{
    QStringList names;
    auto trackerInfo = m_trackers->getTrackerInfo();
    for (const auto &info : std::as_const(trackerInfo))
    {
        names << info.name;
    }
    return names;
}

double Scene::getTrackerValue(const QString &name)
{
    return m_trackers->getTrackerValue(name);
}

QPointF Scene::getTrackerPoint(const QString &name)
{
    return m_trackers->getTrackerPoint(name);
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
        data.animatorData = m_animator->getData().toJson().object();
    }
    if (m_player)
    {
        data.playerData = m_player->getData().toJson().object();
    }
    if (m_parser)
    {
        // Note: Parser getData() method needs to be implemented
        data.parserData = QJsonObject(); // Placeholder
    }
    if (m_trackers)
    {
        data.trackerData = m_trackers->getData().toJson().object();
    }

    // Get all mobjects data
    QJsonArray mobjectsArray;
    for (auto m : m_objects)
    {
        qInfo() << "lOOPING OVER MOBJECTS";
        mobjectsArray.append(m->getData().toJson().object());
    }
    // for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
    // {
    //     if (it.value())
    //     {
    //         QJsonObject mobjectData;
    //         mobjectData["id"] = it.key();
    //         mobjectData["type"] = it.value()->metaObject()->className();
    //         if (it.value()->getProperties())
    //         {
    //             // Note: Need to implement mobject properties serialization
    //             mobjectData["properties"] = QJsonObject(); // Placeholder
    //         }
    //         mobjectsArray.append(mobjectData);
    //     }
    // }
    data.mobjectsData = mobjectsArray;

    return data;
}

void Scene::setFromJSON(const QJsonObject &o)
{
    SceneData data = SceneData::fromJSON(o);

    // Set basic scene properties
    setActiveId(data.activeId);
    gridsize = data.gridSize;
    setbg(data.backgroundColor);
    setShowBorders(data.showBorders);

    // Restore managers state
    if (m_animator && !data.animatorData.isEmpty())
    {
        m_animator->setFromJSON(data.animatorData);
    }
    if (m_player && !data.playerData.isEmpty())
    {
        m_player->setFromJSON(data.playerData);
    }
    if (m_trackers && !data.trackerData.isEmpty())
    {
        m_trackers->setFromJSON(data.trackerData);
    }

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
            getMobject(id)->setfromJSON(mobjectData);
        }
    }
}
