#include "trackermanager.h"
#include "ValueTracker.h"
#include "Scene.h"

TrackerManager::TrackerManager(Scene *c, QObject *parent)
    : canvas(c)
{
    this->setParent(parent);
}

void TrackerManager::addValueTracker(const QString &name, TrackerData *tracker)
{
    if (!tracker || name.isEmpty())
        return;

    if (m_valueTrackers.contains(name))
    {
        removeValueTracker(name);
    }

    m_valueTrackers[name] = tracker;
    emit trackerAdded(name);
}

void TrackerManager::addValueTracker(const QString &name, PtTrackerData *tracker)
{
    if (!tracker || name.isEmpty())
        return;

    if (m_ptvalueTrackers.contains(name))
    {
        removePtValueTracker(name);
    }

    m_ptvalueTrackers[name] = tracker;
    emit trackerAdded(name);
}

void TrackerManager::removeValueTracker(const QString &name)
{
    qInfo()<<"REMOVING";
    if (m_valueTrackers.contains(name))
    {
        TrackerData *tracker = m_valueTrackers.take(name);
        if (tracker && tracker->parent() == this)
        {
            tracker->deleteLater();
        }
        emit trackerRemoved(name);
    }
}

void TrackerManager::removePtValueTracker(const QString &name)
{
    if (m_ptvalueTrackers.contains(name))
    {
        PtTrackerData *tracker = m_ptvalueTrackers.take(name);
        if (tracker && tracker->parent() == this)
        {
            tracker->deleteLater();
        }
        emit trackerRemoved(name);
    }
}

ValueTracker *TrackerManager::getValueTracker(const QString &name) const
{
    TrackerData *data = m_valueTrackers.value(name, nullptr);
    if (!data)
        return nullptr;
    return data->tracker();
}

PtValueTracker *TrackerManager::getPtValueTracker(const QString &name) const
{
    PtTrackerData *data = m_ptvalueTrackers.value(name, nullptr);
    if (!data)
        return nullptr;
    return data->tracker();
}

bool TrackerManager::hasValueTracker(const QString &name) const
{
    return m_valueTrackers.contains(name);
}

bool TrackerManager::hasPointTracker(const QString &name) const
{
    return m_ptvalueTrackers.contains(name);
}

ValueTracker *TrackerManager::createValueTracker(const QString &name, qreal initialValue)
{
    auto existingTracker = getValueTracker(name);
    if (existingTracker)
    {
        return existingTracker;
    }

    TrackerData *tracker = new TrackerData();
    tracker->tracker()->setValue(initialValue);
    tracker->setParentItem(this);
    addValueTracker(name, tracker);
    // qInfo()<<"CREATING TRACKER";
    // qInfo()<<"Trackers : "<<getTrackerNames();
    return tracker->tracker();
}

PtValueTracker *TrackerManager::createValueTracker(const QString &name, QPointF initialValue)
{
    auto existingTracker = getPtValueTracker(name);
    if (existingTracker)
    {
        return existingTracker;
    }

    PtTrackerData *tracker = new PtTrackerData();
    tracker->tracker()->setValue(initialValue);
    tracker->setParentItem(this);
    addValueTracker(name, tracker);
    return tracker->tracker();
}

QStringList TrackerManager::getTrackerNames() const
{
    return m_valueTrackers.keys();
}

QStringList TrackerManager::getPointTrackerNames() const
{
    return m_ptvalueTrackers.keys();
}

qreal TrackerManager::getTrackerMin(const QString &name) const
{
    TrackerData *data = m_valueTrackers.value(name, nullptr);
    return data ? data->min() : 0.0;
}

qreal TrackerManager::getTrackerMax(const QString &name) const
{
    TrackerData *data = m_valueTrackers.value(name, nullptr);
    return data ? data->max() : 0.0;
}

QPointF TrackerManager::getPointTrackerMin(const QString &name) const
{
    PtTrackerData *data = m_ptvalueTrackers.value(name, nullptr);
    return data ? data->min() : QPointF(0, 0);
}

QPointF TrackerManager::getPointTrackerMax(const QString &name) const
{
    PtTrackerData *data = m_ptvalueTrackers.value(name, nullptr);
    return data ? data->max() : QPointF(0, 0);
}

bool TrackerManager::hasSliderConfiguration(const QString &name) const
{
    TrackerData *data = m_valueTrackers.value(name, nullptr);
    return data && (data->min() != data->max());
}

bool TrackerManager::hasPointSliderConfiguration(const QString &name) const
{
    PtTrackerData *data = m_ptvalueTrackers.value(name, nullptr);
    return data && (data->min() != data->max());
}

QList<TrackerData *> TrackerManager::activeTrackers() const
{
    return m_activeTrackers;
}

void TrackerManager::setActiveTrackers(const QList<TrackerData *> &newActiveTrackers)
{
    if (m_activeTrackers == newActiveTrackers)
        return;
    m_activeTrackers = newActiveTrackers;
    emit activeTrackersChanged();
}

QList<PtTrackerData *> TrackerManager::activePtTrackers() const
{
    return m_activePtTrackers;
}

void TrackerManager::setActivePtTrackers(const QList<PtTrackerData *> &newActivePtTrackers)
{
    if (m_activePtTrackers == newActivePtTrackers)
        return;
    m_activePtTrackers = newActivePtTrackers;
    emit activePtTrackersChanged();
}

void TrackerManager::addSlider(const QString &s, qreal maxval, qreal minval, qreal aprtime)
{
    if (!m_valueTrackers.contains(s))
        return;
    m_valueTrackers[s]->setAppeartime(aprtime);
    m_valueTrackers[s]->setMin(minval);
    m_valueTrackers[s]->setMax(maxval);
    emit sliderConfigurationChanged(s);
}

void TrackerManager::addSlider(const QString &s, QPointF maxval, QPointF minval, qreal aprtime)
{
    if (!m_ptvalueTrackers.contains(s))
        return;
    m_ptvalueTrackers[s]->setAppeartime(aprtime);
    m_ptvalueTrackers[s]->setMin(minval);
    m_ptvalueTrackers[s]->setMax(maxval);
    emit sliderConfigurationChanged(s);
}

void TrackerManager::clearAllTrackers()
{
    qInfo()<<"CLEARING ALL TRACKERS";
    QStringList names = m_valueTrackers.keys();
    for (const QString &name : std::as_const(names))
    {
        removeValueTracker(name);
    }
    names = m_ptvalueTrackers.keys();
    for (const QString &name : std::as_const(names))
    {
        removePtValueTracker(name);
    }
}

void TrackerManager::changeActiveTrackers(qreal t)
{
    // Safe removal from completed sets on reverse playback
    auto it = m_completedTrackers.begin();
    while (it != m_completedTrackers.end())
    {
        if ((*it)->appeartime() > t)
        {
            it = m_completedTrackers.erase(it);
        }
        else
        {
            ++it;
        }
    }
    auto itPt = m_completedPtTrackers.begin();
    while (itPt != m_completedPtTrackers.end())
    {
        if ((*itPt)->appeartime() > t)
        {
            itPt = m_completedPtTrackers.erase(itPt);
        }
        else
        {
            ++itPt;
        }
    }

    m_activeTrackers.clear();
    for (auto item : std::as_const(m_valueTrackers))
    {
        if (item->appeartime() <= t && !m_completedTrackers.contains(item))
        {
            m_activeTrackers.append(item);
        }
    }
    m_activePtTrackers.clear();
    for (auto item : std::as_const(m_ptvalueTrackers))
    {
        if (item->appeartime() <= t && !m_completedPtTrackers.contains(item))
        {
            m_activePtTrackers.append(item);
        }
    }

    // Determine next appearance to pause player
    qreal nextAppearTime = std::numeric_limits<qreal>::max();
    for (auto item : std::as_const(m_valueTrackers))
    {
        if (!m_completedTrackers.contains(item) && item->appeartime() < nextAppearTime)
        {
            nextAppearTime = item->appeartime();
        }
    }

    for (auto item : std::as_const(m_ptvalueTrackers))
    {
        if (!m_completedPtTrackers.contains(item) && item->appeartime() < nextAppearTime)
        {
            nextAppearTime = item->appeartime();
        }
    }

    if (nextAppearTime != std::numeric_limits<qreal>::max() && t >= nextAppearTime)
    {
        canvas->player()->pause();
    }

    emit activeTrackersChanged();
    emit activePtTrackersChanged();
}

void TrackerManager::Continue()
{
    for (auto item : std::as_const(m_activeTrackers))
    {
        m_completedTrackers.insert(item);
    }
    for (auto item : std::as_const(m_activePtTrackers))
    {
        m_completedPtTrackers.insert(item);
    }
    m_activeTrackers.clear();
    m_activePtTrackers.clear();

    emit activeTrackersChanged();
    emit activePtTrackersChanged();

    canvas->player()->play();
}

