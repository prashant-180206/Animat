#include "trackermanager.h"
#include "Scene.h"
#include "Math/Mobjects/Base/ClickableMobject.h"
#include "../Managers/funcmap.h"
#include <QDebug>

TrackerManager::TrackerManager(Scene *canvas, QObject *parent)
    : QObject(parent), m_canvas(canvas), m_parser(new Parser()), m_scriptParser(new AnimationScriptParser()), m_dependencyGraph(new DependencyGraph())
{
    // Set up default time variable
    m_parser->setVariable("t", 0.0);
}

TrackerManager::~TrackerManager()
{
    clearAllTrackers();
    delete m_parser;
    delete m_scriptParser;
    delete m_dependencyGraph;
}

void TrackerManager::parseScript(const QString &script)
{
    qDebug() << "TrackerManager::parseScript() called with:" << script;

    // Parse the script into commands
    QVector<TrackerCommand> commands = m_scriptParser->parseScript(script);
    qDebug() << "TrackerManager::parseScript() - Parsed" << commands.size() << "commands";

    // Clear existing data
    m_dependencyGraph->clear();

    // Execute all commands
    for (const TrackerCommand &command : commands)
    {
        qDebug() << "TrackerManager::parseScript() - Executing command type:" << static_cast<int>(command.getType()) << "name:" << command.getName();
        executeCommand(command);
    }

    // Check for circular dependencies
    if (m_dependencyGraph->hasCircularDependency())
    {
        qWarning() << "Circular dependency detected in tracker script!";
        m_dependencyGraph->printGraph();
    }

    // Initial update of all trackers
    updateRegularExpressionTrackers();
    updateDynamicTrackers();
    updateTrackerInfo();
}

void TrackerManager::executeCommand(const TrackerCommand &command)
{
    switch (command.getType())
    {
    case TrackerCommand::VALUE_TRACKER:
    case TrackerCommand::EXPRESSION_TRACKER:
    case TrackerCommand::DYNAMIC_VALUE_TRACKER:
        createValueTracker(command);
        break;

    case TrackerCommand::POINT_TRACKER:
    case TrackerCommand::POINT_EXPRESSION_TRACKER:
    case TrackerCommand::DYNAMIC_POINT_TRACKER:
        createPointTracker(command);
        break;

    case TrackerCommand::CONNECTION:
        createConnection(command);
        break;
    }
}

void TrackerManager::createValueTracker(const TrackerCommand &command)
{
    const QString &name = command.getName();

    // Remove existing tracker if present
    if (m_valueTrackers.contains(name))
    {
        m_valueTrackers[name]->deleteLater();
        m_valueTrackers.remove(name);
    }

    // Create new tracker
    ValueTracker *tracker = new ValueTracker();
    // qDebug() << "TrackerManager: Creating new VALUE tracker with name:" << name;
    m_valueTrackers[name] = tracker;
    // qDebug() << "TrackerManager: VALUE tracker added. Hash size now:" << m_valueTrackers.size();

    // Connect to update signals
    connect(tracker, QOverload<double>::of(&ValueTracker::valueChanged),
            this, &TrackerManager::onTrackerValueChanged); // Set up based on command type
    switch (command.getType())
    {
    case TrackerCommand::VALUE_TRACKER:
        tracker->setValue(command.getValue());
        break;

    case TrackerCommand::EXPRESSION_TRACKER:
        m_regularExpressions[name] = command.getExpression();
        // Initial evaluation
        tracker->setValue(m_parser->evaluate(command.getExpression()));
        break;

    case TrackerCommand::DYNAMIC_VALUE_TRACKER:
        // qDebug() << "TrackerManager: Creating DYNAMIC_VALUE_TRACKER:" << name;
        // qDebug() << "  - Expression:" << command.getExpression();
        // qDebug() << "  - Dependencies:" << command.getDependencies();

        m_dynamicExpressions[name] = command.getExpression();
        m_dependencyGraph->addDependency(name, command.getDependencies());
        // Initial evaluation with current tracker values
        {
            QHash<QString, double> currentValues = getCurrentTrackerValues();
            // qDebug() << "  - Current tracker values:" << currentValues;

            double value = m_parser->evaluateDynamic(command.getExpression(), currentValues);
            // qDebug() << "  - Evaluated result:" << value;

            tracker->setValue(value);
            // qDebug() << "  - Tracker value set to:" << tracker->value();
        }
        break;
    default:
        break;
    }

    // Emit signal after all setup is complete (including dynamic calculations)
    emit trackersListChanged();
}

void TrackerManager::createPointTracker(const TrackerCommand &command)
{
    const QString &name = command.getName();

    // Remove existing tracker if present
    if (m_pointTrackers.contains(name))
    {
        m_pointTrackers[name]->deleteLater();
        m_pointTrackers.remove(name);
    }

    // Create new tracker
    PtValueTracker *tracker = new PtValueTracker();
    qDebug() << "TrackerManager: Creating new POINT tracker with name:" << name;
    m_pointTrackers[name] = tracker;
    qDebug() << "TrackerManager: POINT tracker added. Hash size now:" << m_pointTrackers.size();
    emit trackersListChanged(); // Emit when new tracker added

    // Connect to update signals
    connect(tracker, &PtValueTracker::valueChanged,
            this, &TrackerManager::onTrackerPointChanged);

    // Set up based on command type
    switch (command.getType())
    {
    case TrackerCommand::POINT_TRACKER:
        tracker->setValue(command.getPoint());
        break;

    case TrackerCommand::POINT_EXPRESSION_TRACKER:
        m_regularPointExpressions[name] = qMakePair(command.getExpressionX(), command.getExpressionY());
        // Initial evaluation
        {
            double x = m_parser->evaluate(command.getExpressionX());
            double y = m_parser->evaluate(command.getExpressionY());
            tracker->setValue(QPointF(x, y));
        }
        break;

    case TrackerCommand::DYNAMIC_POINT_TRACKER:
        m_dynamicPointExpressions[name] = qMakePair(command.getExpressionX(), command.getExpressionY());
        m_dependencyGraph->addDependency(name, command.getDependencies());
        // Initial evaluation with current tracker values
        {
            QHash<QString, double> currentValues = getCurrentTrackerValues();
            double x = m_parser->evaluateDynamic(command.getExpressionX(), currentValues);
            double y = m_parser->evaluateDynamic(command.getExpressionY(), currentValues);
            tracker->setValue(QPointF(x, y));
        }
        break;

    default:
        break;
    }
}

void TrackerManager::createConnection(const TrackerCommand &command)
{
    const QString &trackerName = command.getName();
    const QString &objectName = command.getObjectName();
    const QString &propertyName = command.getPropertyName();

    if (!m_canvas)
    {
        qWarning() << "Cannot create connection: no canvas available";
        return;
    }

    ClickableMobject *object = m_canvas->getMobject(objectName);
    if (!object)
    {
        qWarning() << "Cannot find object:" << objectName;
        return;
    }

    connectTracker(trackerName, object, propertyName);
}

void TrackerManager::connectTracker(const QString &trackerName, ClickableMobject *object, const QString &property)
{
    if (!object)
        return;

    // Store connection for tracking
    Connection conn{object, property};
    m_connections[trackerName].append(conn);

    // Set up connection based on tracker type
    if (m_valueTrackers.contains(trackerName))
    {
        ValueTracker *tracker = m_valueTrackers[trackerName];
        connect(tracker, &ValueTracker::valueChanged, object, [=](double value)
                {
            auto func = FuncMap::ConnectFunc[property];
            if (func) {
                func(object, value);
            } });
    }
    else if (m_pointTrackers.contains(trackerName))
    {
        PtValueTracker *tracker = m_pointTrackers[trackerName];
        connect(tracker, &PtValueTracker::valueChanged, object, [=](QPointF point)
                {
            auto func = FuncMap::PtConnectFunc[property];
            if (func) {
                func(object, point);
            } });
    }
}

void TrackerManager::setMasterVariable(const QString &name, double value)
{
    m_parser->setVariable(name, value);

    // Update regular expression trackers first
    updateRegularExpressionTrackers();

    // Then update dynamic trackers (which may depend on regular expression trackers)
    updateDynamicTrackers();

    updateTrackerInfo();
}

void TrackerManager::updateTracker(const QString &name, double value)
{
    if (m_valueTrackers.contains(name))
    {
        m_valueTrackers[name]->setValue(value);

        // Update all trackers that depend on this one
        QStringList affected = m_dependencyGraph->getUpdateOrder(name);
        for (const QString &affectedName : affected)
        {
            if (affectedName != name)
            { // Don't update the tracker we just changed
                updateSingleDynamicTracker(affectedName);
            }
        }

        updateTrackerInfo();
    }
}

void TrackerManager::updateRegularExpressionTrackers()
{
    // Update value trackers with regular expressions
    for (auto it = m_regularExpressions.begin(); it != m_regularExpressions.end(); ++it)
    {
        const QString &name = it.key();
        const QString &expression = it.value();

        if (m_valueTrackers.contains(name))
        {
            double value = m_parser->evaluate(expression);
            m_valueTrackers[name]->setValue(value);
        }
    }

    // Update point trackers with regular expressions
    for (auto it = m_regularPointExpressions.begin(); it != m_regularPointExpressions.end(); ++it)
    {
        const QString &name = it.key();
        const QPair<QString, QString> &expressions = it.value();

        if (m_pointTrackers.contains(name))
        {
            double x = m_parser->evaluate(expressions.first);
            double y = m_parser->evaluate(expressions.second);
            m_pointTrackers[name]->setValue(QPointF(x, y));
        }
    }
}

void TrackerManager::updateDynamicTrackers()
{
    // Get all trackers in dependency order
    QStringList updateOrder = m_dependencyGraph->getTopologicalOrder();

    for (const QString &name : updateOrder)
    {
        updateSingleDynamicTracker(name);
    }
}

void TrackerManager::updateSingleDynamicTracker(const QString &name)
{
    QHash<QString, double> currentValues = getCurrentTrackerValues();

    // Update dynamic value tracker
    if (m_dynamicExpressions.contains(name) && m_valueTrackers.contains(name))
    {
        const QString &expression = m_dynamicExpressions[name];
        double value = m_parser->evaluateDynamic(expression, currentValues);
        m_valueTrackers[name]->setValue(value);
    }

    // Update dynamic point tracker
    if (m_dynamicPointExpressions.contains(name) && m_pointTrackers.contains(name))
    {
        const QPair<QString, QString> &expressions = m_dynamicPointExpressions[name];
        double x = m_parser->evaluateDynamic(expressions.first, currentValues);
        double y = m_parser->evaluateDynamic(expressions.second, currentValues);
        m_pointTrackers[name]->setValue(QPointF(x, y));
    }
}

QHash<QString, double> TrackerManager::getCurrentTrackerValues() const
{
    QHash<QString, double> values;

    for (auto it = m_valueTrackers.begin(); it != m_valueTrackers.end(); ++it)
    {
        values[it.key()] = it.value()->value();
    }

    return values;
}

QVector<TrackerManager::TrackerInfo> TrackerManager::getTrackerInfo() const
{
    return m_trackerInfo;
}

void TrackerManager::updateTrackerInfo()
{
    m_trackerInfo.clear();

    // Add value trackers
    for (auto it = m_valueTrackers.begin(); it != m_valueTrackers.end(); ++it)
    {
        m_trackerInfo.append(createTrackerInfo(it.key()));
    }

    // Add point trackers
    for (auto it = m_pointTrackers.begin(); it != m_pointTrackers.end(); ++it)
    {
        m_trackerInfo.append(createTrackerInfo(it.key()));
    }

    emit trackerInfoChanged();
}

TrackerManager::TrackerInfo TrackerManager::createTrackerInfo(const QString &name) const
{
    TrackerInfo info;
    info.name = name;
    info.isDynamic = m_dynamicExpressions.contains(name) || m_dynamicPointExpressions.contains(name);
    info.isExpression = info.isDynamic ||
                        m_regularExpressions.contains(name) ||
                        m_regularPointExpressions.contains(name);

    if (m_valueTrackers.contains(name))
    {
        info.displayValue = QString::number(m_valueTrackers[name]->value(), 'f', 2);
        if (m_dynamicExpressions.contains(name))
        {
            info.expression = m_dynamicExpressions[name];
        }
        else if (m_regularExpressions.contains(name))
        {
            info.expression = m_regularExpressions[name];
        }
    }
    else if (m_pointTrackers.contains(name))
    {
        QPointF point = m_pointTrackers[name]->value();
        info.displayValue = QString("(%1, %2)").arg(point.x(), 0, 'f', 2).arg(point.y(), 0, 'f', 2);
        if (m_dynamicPointExpressions.contains(name))
        {
            QPair<QString, QString> exprs = m_dynamicPointExpressions[name];
            info.expression = QString("(%1, %2)").arg(exprs.first, exprs.second);
        }
        else if (m_regularPointExpressions.contains(name))
        {
            QPair<QString, QString> exprs = m_regularPointExpressions[name];
            info.expression = QString("(%1, %2)").arg(exprs.first, exprs.second);
        }
    }

    return info;
}

void TrackerManager::clearAllTrackers()
{
    // Clean up value trackers
    for (ValueTracker *tracker : m_valueTrackers)
    {
        tracker->deleteLater();
    }
    m_valueTrackers.clear();

    // Clean up point trackers
    for (PtValueTracker *tracker : m_pointTrackers)
    {
        tracker->deleteLater();
    }
    m_pointTrackers.clear();

    // Clear other data
    m_dynamicExpressions.clear();
    m_dynamicPointExpressions.clear();
    m_regularExpressions.clear();
    m_regularPointExpressions.clear();
    m_connections.clear();
    m_trackerInfo.clear();

    m_dependencyGraph->clear();

    emit trackerInfoChanged();
}

void TrackerManager::onTrackerValueChanged(double value)
{
    // Find which tracker changed
    ValueTracker *sender = qobject_cast<ValueTracker *>(QObject::sender());
    if (!sender)
        return;

    QString trackerName;
    for (auto it = m_valueTrackers.begin(); it != m_valueTrackers.end(); ++it)
    {
        if (it.value() == sender)
        {
            trackerName = it.key();
            break;
        }
    }

    if (!trackerName.isEmpty())
    {
        emit trackerValueChanged(trackerName, value);
        updateConnectedObjects(trackerName);
    }
}

void TrackerManager::onTrackerPointChanged(QPointF point)
{
    // Find which tracker changed
    PtValueTracker *sender = qobject_cast<PtValueTracker *>(QObject::sender());
    if (!sender)
        return;

    QString trackerName;
    for (auto it = m_pointTrackers.begin(); it != m_pointTrackers.end(); ++it)
    {
        if (it.value() == sender)
        {
            trackerName = it.key();
            break;
        }
    }

    if (!trackerName.isEmpty())
    {
        emit trackerPointChanged(trackerName, point);
        updateConnectedObjects(trackerName);
    }
}

void TrackerManager::updateConnectedObjects(const QString &trackerName)
{
    if (m_connections.contains(trackerName))
    {
        const QVector<Connection> &connections = m_connections[trackerName];

        if (m_valueTrackers.contains(trackerName))
        {
            double value = m_valueTrackers[trackerName]->value();
            for (const Connection &conn : connections)
            {
                auto func = FuncMap::ConnectFunc[conn.property];
                if (func)
                {
                    func(conn.object, value);
                }
            }
        }
        else if (m_pointTrackers.contains(trackerName))
        {
            QPointF point = m_pointTrackers[trackerName]->value();
            for (const Connection &conn : connections)
            {
                auto func = FuncMap::PtConnectFunc[conn.property];
                if (func)
                {
                    func(conn.object, point);
                }
            }
        }
    }
}

// Getter implementations
ValueTracker *TrackerManager::getValueTracker(const QString &name) const
{
    return m_valueTrackers.value(name, nullptr);
}

PtValueTracker *TrackerManager::getPointTracker(const QString &name) const
{
    return m_pointTrackers.value(name, nullptr);
}

double TrackerManager::getTrackerValue(const QString &name) const
{
    ValueTracker *tracker = m_valueTrackers.value(name, nullptr);
    return tracker ? tracker->value() : 0.0;
}

QPointF TrackerManager::getTrackerPoint(const QString &name) const
{
    PtValueTracker *tracker = m_pointTrackers.value(name, nullptr);
    return tracker ? tracker->value() : QPointF(0, 0);
}

QStringList TrackerManager::getAllValueTrackerNames() const
{
    QStringList names = m_valueTrackers.keys();
    // qDebug() << "TrackerManager::getAllValueTrackerNames() - Count:" << names.size() << "Names:" << names;
    // qDebug() << "TrackerManager: m_valueTrackers hash size:" << m_valueTrackers.size();
    // for (auto it = m_valueTrackers.begin(); it != m_valueTrackers.end(); ++it)
    // {
    //     qDebug() << "  - Key:" << it.key() << "Value ptr:" << it.value();
    // }
    return names;
}

QStringList TrackerManager::getAllPointTrackerNames() const
{
    QStringList names = m_pointTrackers.keys();
    // qDebug() << "TrackerManager::getAllPointTrackerNames() - Count:" << names.size() << "Names:" << names;
    return names;
}
