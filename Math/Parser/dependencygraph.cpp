#include "dependencygraph.h"
#include <QDebug>
#include <QQueue>

DependencyGraph::DependencyGraph()
{
}

void DependencyGraph::addDependency(const QString &dependent, const QString &dependency)
{
    if (!m_dependencies[dependent].contains(dependency))
    {
        m_dependencies[dependent].append(dependency);
    }
    if (!m_dependents[dependency].contains(dependent))
    {
        m_dependents[dependency].append(dependent);
    }
}

void DependencyGraph::addDependency(const QString &dependent, const QStringList &dependencies)
{
    for (const QString &dependency : dependencies)
    {
        addDependency(dependent, dependency);
    }
}

void DependencyGraph::removeDependencies(const QString &tracker)
{
    // Remove from forward dependencies
    if (m_dependencies.contains(tracker))
    {
        QStringList deps = m_dependencies[tracker];
        m_dependencies.remove(tracker);

        // Remove from reverse dependencies
        for (const QString &dep : deps)
        {
            if (m_dependents.contains(dep))
            {
                m_dependents[dep].removeAll(tracker);
                if (m_dependents[dep].isEmpty())
                {
                    m_dependents.remove(dep);
                }
            }
        }
    }

    // Remove tracker from being a dependent of others
    if (m_dependents.contains(tracker))
    {
        m_dependents.remove(tracker);
    }

    // Remove tracker from other trackers' dependency lists
    for (auto &deps : m_dependencies)
    {
        deps.removeAll(tracker);
    }
}

void DependencyGraph::clear()
{
    m_dependencies.clear();
    m_dependents.clear();
}

QStringList DependencyGraph::getDependencies(const QString &tracker) const
{
    return m_dependencies.value(tracker, QStringList());
}

QStringList DependencyGraph::getDependents(const QString &tracker) const
{
    return m_dependents.value(tracker, QStringList());
}

QStringList DependencyGraph::getUpdateOrder(const QString &changedTracker) const
{
    QStringList affected = getAllAffected(changedTracker);

    // Sort affected trackers in topological order
    QStringList ordered;
    QHash<QString, int> inDegree;

    // Calculate in-degrees for affected trackers only
    for (const QString &tracker : affected)
    {
        inDegree[tracker] = 0;
        QStringList deps = getDependencies(tracker);
        for (const QString &dep : deps)
        {
            if (affected.contains(dep))
            {
                inDegree[tracker]++;
            }
        }
    }

    // Topological sort using Kahn's algorithm
    QQueue<QString> queue;
    for (const QString &tracker : affected)
    {
        if (inDegree[tracker] == 0)
        {
            queue.enqueue(tracker);
        }
    }

    while (!queue.isEmpty())
    {
        QString current = queue.dequeue();
        ordered.append(current);

        QStringList dependents = getDependents(current);
        for (const QString &dependent : dependents)
        {
            if (affected.contains(dependent))
            {
                inDegree[dependent]--;
                if (inDegree[dependent] == 0)
                {
                    queue.enqueue(dependent);
                }
            }
        }
    }

    return ordered;
}

QStringList DependencyGraph::getAllAffected(const QString &changedTracker) const
{
    QSet<QString> visited;
    QStringList result;
    getAffectedDFS(changedTracker, visited, result);
    return result;
}

void DependencyGraph::getAffectedDFS(const QString &tracker, QSet<QString> &visited, QStringList &result) const
{
    if (visited.contains(tracker))
    {
        return;
    }

    visited.insert(tracker);
    result.append(tracker);

    QStringList dependents = getDependents(tracker);
    for (const QString &dependent : dependents)
    {
        getAffectedDFS(dependent, visited, result);
    }
}

bool DependencyGraph::hasCircularDependency() const
{
    QSet<QString> visited;
    QSet<QString> recursionStack;

    for (auto it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
    {
        const QString &tracker = it.key();
        if (!visited.contains(tracker))
        {
            if (hasCircularDependencyDFS(tracker, visited, recursionStack))
            {
                return true;
            }
        }
    }

    return false;
}

bool DependencyGraph::hasCircularDependency(const QString &tracker) const
{
    QSet<QString> visited;
    QSet<QString> recursionStack;
    return hasCircularDependencyDFS(tracker, visited, recursionStack);
}

bool DependencyGraph::hasCircularDependencyDFS(const QString &tracker, QSet<QString> &visited, QSet<QString> &recursionStack) const
{
    visited.insert(tracker);
    recursionStack.insert(tracker);

    QStringList dependencies = getDependencies(tracker);
    for (const QString &dependency : dependencies)
    {
        if (!visited.contains(dependency))
        {
            if (hasCircularDependencyDFS(dependency, visited, recursionStack))
            {
                return true;
            }
        }
        else if (recursionStack.contains(dependency))
        {
            return true; // Back edge found - circular dependency
        }
    }

    recursionStack.remove(tracker);
    return false;
}

QStringList DependencyGraph::getTopologicalOrder() const
{
    QStringList ordered;
    QHash<QString, int> inDegree;

    // Get all unique trackers
    QSet<QString> allTrackers;
    for (auto it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
    {
        allTrackers.insert(it.key());
        for (const QString &dep : it.value())
        {
            allTrackers.insert(dep);
        }
    }

    // Calculate in-degrees
    for (const QString &tracker : allTrackers)
    {
        inDegree[tracker] = getDependencies(tracker).size();
    }

    // Topological sort using Kahn's algorithm
    QQueue<QString> queue;
    for (const QString &tracker : allTrackers)
    {
        if (inDegree[tracker] == 0)
        {
            queue.enqueue(tracker);
        }
    }

    while (!queue.isEmpty())
    {
        QString current = queue.dequeue();
        ordered.append(current);

        QStringList dependents = getDependents(current);
        for (const QString &dependent : dependents)
        {
            inDegree[dependent]--;
            if (inDegree[dependent] == 0)
            {
                queue.enqueue(dependent);
            }
        }
    }

    return ordered;
}

void DependencyGraph::printGraph() const
{
    qDebug() << "Dependency Graph:";
    for (auto it = m_dependencies.begin(); it != m_dependencies.end(); ++it)
    {
        qDebug() << it.key() << "depends on:" << it.value();
    }

    qDebug() << "\nReverse Dependencies:";
    for (auto it = m_dependents.begin(); it != m_dependents.end(); ++it)
    {
        qDebug() << it.key() << "is depended on by:" << it.value();
    }
}