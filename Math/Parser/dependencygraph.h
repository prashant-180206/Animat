#ifndef DEPENDENCYGRAPH_H
#define DEPENDENCYGRAPH_H

#include <QString>
#include <QHash>
#include <QStringList>
#include <QSet>

class DependencyGraph
{
public:
    DependencyGraph();

    // Add a dependency relationship: dependent depends on dependency
    void addDependency(const QString &dependent, const QString &dependency);
    void addDependency(const QString &dependent, const QStringList &dependencies);

    // Remove dependencies
    void removeDependencies(const QString &tracker);
    void clear();

    // Get dependencies and dependents
    QStringList getDependencies(const QString &tracker) const;
    QStringList getDependents(const QString &tracker) const;

    // Get update order: when tracker changes, what order should others be updated?
    QStringList getUpdateOrder(const QString &changedTracker) const;

    // Get all trackers that need to be updated when given tracker changes
    QStringList getAllAffected(const QString &changedTracker) const;

    // Check for circular dependencies
    bool hasCircularDependency() const;
    bool hasCircularDependency(const QString &tracker) const;

    // Get topological order of all trackers (for initialization)
    QStringList getTopologicalOrder() const;

    // Debug
    void printGraph() const;

private:
    // Forward dependencies: tracker -> [trackers it depends on]
    QHash<QString, QStringList> m_dependencies;

    // Reverse dependencies: tracker -> [trackers that depend on it]
    QHash<QString, QStringList> m_dependents;

    // Helper methods
    void buildReverseDependencies();
    bool hasCircularDependencyDFS(const QString &tracker, QSet<QString> &visited, QSet<QString> &recursionStack) const;
    void getAffectedDFS(const QString &tracker, QSet<QString> &visited, QStringList &result) const;
};

#endif // DEPENDENCYGRAPH_H