#include "trackermanager.h"
#include "../ValueTracker/ValueTracker.h"

void TrackerManager::addValueTracker(const QString &name, ValueTracker *tracker)
{
    if (!tracker || name.isEmpty())
        return;

    // Remove existing tracker with same name if it exists
    if (m_valueTrackers.contains(name))
    {
        removeValueTracker(name);
    }

    m_valueTrackers[name] = tracker;
    emit trackerAdded(name);
}

void TrackerManager::addValueTracker(const QString &name, PtValueTracker *tracker)
{
    if (!tracker || name.isEmpty())
        return;

    // Remove existing tracker with same name if it exists
    if (m_valueTrackers.contains(name))
    {
        removeValueTracker(name);
    }

    m_ptvalueTrackers[name] = tracker;
    emit trackerAdded(name);
}

void TrackerManager::removeValueTracker(const QString &name)
{
    if (m_valueTrackers.contains(name))
    {
        ValueTracker *tracker = m_valueTrackers.take(name);
        if (tracker && tracker->parent() == this)
        {
            tracker->deleteLater();
        }
        emit trackerRemoved(name);
    }
}

void TrackerManager::removePtValueTracker(const QString &name)
{
    if (m_valueTrackers.contains(name))
    {
        PtValueTracker *tracker = m_ptvalueTrackers.take(name);
        if (tracker && tracker->parent() == this)
        {
            tracker->deleteLater();
        }
        emit trackerRemoved(name);
    }
}



ValueTracker *TrackerManager::getValueTracker(const QString &name) const
{
    return m_valueTrackers.value(name, nullptr);
}

PtValueTracker *TrackerManager::getPtValueTracker(const QString &name) const
{
    return m_ptvalueTrackers.value(name, nullptr);
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
        return existingTracker; // Return existing tracker if it already exists
    }

    ValueTracker *tracker = new ValueTracker();
    tracker->setValue(initialValue);
    tracker->setParentItem(this);
    addValueTracker(name, tracker);
    return tracker;
}

PtValueTracker *TrackerManager::createValueTracker(const QString &name, QPointF initialValue)
{
    auto existingTracker = getPtValueTracker(name);
    if (existingTracker)
    {
        return existingTracker; // Return existing tracker if it already exists
    }

    PtValueTracker *tracker = new PtValueTracker();
    tracker->setValue(initialValue);
    tracker->setParentItem(this);
    addValueTracker(name, tracker);
    return tracker;
}

QStringList TrackerManager::getTrackerNames() const
{
    return m_valueTrackers.keys();
}

QStringList TrackerManager::getPointTrackerNames() const
{
    return m_ptvalueTrackers.keys();
}

void TrackerManager::clearAllTrackers()
{
    QStringList names = m_valueTrackers.keys();
    for (const QString &name : std::as_const(names))
    {
        removeValueTracker(name);
    }
}
