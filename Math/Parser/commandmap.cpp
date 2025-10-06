#include "commandmap.h"
#include "trackermanager.h"
#include "scene.h"
#include "../ValueTracker/valuetracker.h"
#include <QDebug>

CommandManager::CommandManager(QObject *parent)
    : QObject{parent}
{
    // Register all default commands on construction
    registerAllDefaultCommands();
}

void CommandManager::registerCommand(const QString &name, const QRegularExpression &regex, Command::CommandHandler handler)
{
    registerCommandWithInfo(name, regex, handler, "Custom", "User defined command", name + " [args]");
}

void CommandManager::registerCommand(const QString &name, const QString &regexPattern, Command::CommandHandler handler)
{
    QRegularExpression regex(regexPattern);
    registerCommand(name, regex, handler);
}

void CommandManager::registerCommandWithInfo(const QString &name, const QRegularExpression &regex,
                                             Command::CommandHandler handler, const QString &category,
                                             const QString &description, const QString &usage)
{
    if (m_commands.contains(name))
    {
        qDebug() << "Warning: Overriding existing command:" << name;
        unregisterCommand(name);
    }

    auto command = std::make_unique<Command>(name, regex, handler);
    auto commandInfo = std::make_unique<CommandInfo>(std::move(command), category, description, usage);

    m_commands[name] = std::move(commandInfo);
    emit commandRegistered(name);
}

void CommandManager::unregisterCommand(const QString &name)
{
    if (m_commands.remove(name))
    {
        emit commandUnregistered(name);
    }
}

void CommandManager::clearAllCommands()
{
    QStringList names = m_commands.keys();
    m_commands.clear();
    for (const QString &name : names)
    {
        emit commandUnregistered(name);
    }
}

bool CommandManager::executeCommand(const QString &input, TrackerManager *trackerManager, Scene *scene)
{
    if (input.trimmed().isEmpty())
    {
        emit commandFailed(input, "Empty command");
        return false;
    }

    QString matches;
    Command *matchedCommand = findMatchingCommand(input, matches);

    if (matchedCommand)
    {
        try
        {
            matchedCommand->execute(matches, trackerManager, scene);
            emit commandExecuted(matchedCommand->name(), input);
            return true;
        }
        catch (const std::exception &e)
        {
            emit commandFailed(input, QString("Execution error: %1").arg(e.what()));
            return false;
        }
    }
    else
    {
        emit commandFailed(input, "No matching command found");
        return false;
    }
}

Command *CommandManager::findMatchingCommand(const QString &input, QString &matches) const
{
    for (auto it = m_commands.begin(); it != m_commands.end(); ++it)
    {
        if (it.value()->command->matches(input, matches))
        {
            return it.value()->command.get();
        }
    }
    return nullptr;
}

bool CommandManager::hasCommand(const QString &name) const
{
    return m_commands.contains(name);
}

QStringList CommandManager::getCommandNames() const
{
    return m_commands.keys();
}

QStringList CommandManager::getAvailableCommands() const
{
    QStringList commands;
    for (auto it = m_commands.begin(); it != m_commands.end(); ++it)
    {
        commands << QString("%1 - %2").arg(it.key(), it.value()->description);
    }
    return commands;
}

QString CommandManager::getCommandHelp(const QString &name) const
{
    if (m_commands.contains(name))
    {
        const auto &info = m_commands[name];
        return QString("Command: %1\nCategory: %2\nDescription: %3\nUsage: %4")
            .arg(name, info->category, info->description, info->usage);
    }
    return QString("Command '%1' not found").arg(name);
}

QStringList CommandManager::getCommandsInCategory(const QString &category) const
{
    QStringList commands;
    for (auto it = m_commands.begin(); it != m_commands.end(); ++it)
    {
        if (it.value()->category == category)
        {
            commands << it.key();
        }
    }
    return commands;
}

void CommandManager::registerBasicCommands()
{
    // create_tracker command
    registerCommandWithInfo("create_tracker", QRegularExpression(R"(^create_tracker\s+(\w+)(?:\s+([+-]?\d*\.?\d+))?\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            // Extract name and value from matches string
            QRegularExpression regex(R"(^create_tracker\s+(\w+)(?:\s+([+-]?\d*\.?\d+))?\s*$)");
            QRegularExpressionMatch match = regex.match(matches);
            if (match.hasMatch()) {
                QString name = match.captured(1);
                qreal initialValue = match.captured(2).isEmpty() ? 0.0 : match.captured(2).toDouble();
                
                if (tm->hasValueTracker(name)) {
                    qDebug() << "Tracker" << name << "already exists";
                    return;
                }
                
                tm->createValueTracker(name, initialValue);
                qDebug() << "Created tracker:" << name << "with initial value:" << initialValue;
            } }, "Basic", "Create a new value tracker", "create_tracker <name> [initial_value]");

    // set_tracker command
    registerCommandWithInfo("set_tracker", QRegularExpression(R"(^set_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            QRegularExpression regex(R"(^set_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)");
            QRegularExpressionMatch match = regex.match(matches);
            if (match.hasMatch()) {
                QString name = match.captured(1);
                qreal value = match.captured(2).toDouble();
                
                ValueTracker* tracker = tm->getValueTracker(name);
                if (tracker) {
                    tracker->setValue(value);
                    qDebug() << "Set tracker" << name << "to value:" << value;
                } else {
                    qDebug() << "Tracker" << name << "not found";
                }
            } }, "Basic", "Set the value of an existing tracker", "set_tracker <name> <value>");

    // list_trackers command
    registerCommandWithInfo("list_trackers", QRegularExpression(R"(^list_trackers\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            QStringList trackerNames = tm->getTrackerNames();
            if (trackerNames.isEmpty()) {
                qDebug() << "No trackers found";
            } else {
                qDebug() << "Trackers:" << trackerNames.join(", ");
            } }, "Basic", "List all existing trackers", "list_trackers");

    // remove_tracker command
    registerCommandWithInfo("remove_tracker", QRegularExpression(R"(^remove_tracker\s+(\w+)\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            QRegularExpression regex(R"(^remove_tracker\s+(\w+)\s*$)");
            QRegularExpressionMatch match = regex.match(matches);
            if (match.hasMatch()) {
                QString name = match.captured(1);
                if (tm->hasValueTracker(name)) {
                    tm->removeValueTracker(name);
                    qDebug() << "Removed tracker:" << name;
                } else {
                    qDebug() << "Tracker" << name << "not found";
                }
            } }, "Basic", "Remove an existing tracker", "remove_tracker <name>");
}

void CommandManager::registerMathCommands()
{
    // add_to_tracker command
    registerCommandWithInfo("add_to_tracker", QRegularExpression(R"(^add_to_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            QRegularExpression regex(R"(^add_to_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)");
            QRegularExpressionMatch match = regex.match(matches);
            if (match.hasMatch()) {
                QString name = match.captured(1);
                qreal addValue = match.captured(2).toDouble();
                
                ValueTracker* tracker = tm->getValueTracker(name);
                if (tracker) {
                    qreal newValue = tracker->value() + addValue;
                    tracker->setValue(newValue);
                    qDebug() << "Added" << addValue << "to tracker" << name << ", new value:" << newValue;
                } else {
                    qDebug() << "Tracker" << name << "not found";
                }
            } }, "Math", "Add a value to an existing tracker", "add_to_tracker <name> <value>");

    // multiply_tracker command
    registerCommandWithInfo("multiply_tracker", QRegularExpression(R"(^multiply_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            QRegularExpression regex(R"(^multiply_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s*$)");
            QRegularExpressionMatch match = regex.match(matches);
            if (match.hasMatch()) {
                QString name = match.captured(1);
                qreal factor = match.captured(2).toDouble();
                
                ValueTracker* tracker = tm->getValueTracker(name);
                if (tracker) {
                    qreal newValue = tracker->value() * factor;
                    tracker->setValue(newValue);
                    qDebug() << "Multiplied tracker" << name << "by" << factor << ", new value:" << newValue;
                } else {
                    qDebug() << "Tracker" << name << "not found";
                }
            } }, "Math", "Multiply tracker value by a factor", "multiply_tracker <name> <factor>");
}

void CommandManager::registerAnimationCommands()
{
    // animate_tracker command
    registerCommandWithInfo("animate_tracker", QRegularExpression(R"(^animate_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s+(\d+)\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            QRegularExpression regex(R"(^animate_tracker\s+(\w+)\s+([+-]?\d*\.?\d+)\s+(\d+)\s*$)");
            QRegularExpressionMatch match = regex.match(matches);
            if (match.hasMatch()) {
                QString name = match.captured(1);
                qreal targetValue = match.captured(2).toDouble();
                int duration = match.captured(3).toInt();
                
                ValueTracker* tracker = tm->getValueTracker(name);
                if (tracker) {
                    qDebug() << "Starting animation for tracker" << name 
                             << "to value" << targetValue 
                             << "over" << duration << "ms";
                    // For now, just set the value directly
                    tracker->setValue(targetValue);
                } else {
                    qDebug() << "Tracker" << name << "not found";
                }
            } }, "Animation", "Animate tracker to target value", "animate_tracker <name> <target_value> <duration_ms>");
}

void CommandManager::registerSceneCommands()
{
    // reset_scene command
    registerCommandWithInfo("reset_scene", QRegularExpression(R"(^reset_scene\s*$)"), [](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            tm->clearAllTrackers();
            qDebug() << "Scene reset - all trackers cleared"; }, "Scene", "Reset scene by clearing all trackers", "reset_scene");

    // help command
    registerCommandWithInfo("help", QRegularExpression(R"(^help(?:\s+(\w+))?\s*$)"), [this](const QString &matches, TrackerManager *tm, Scene *scene)
                            {
            QRegularExpression regex(R"(^help(?:\s+(\w+))?\s*$)");
            QRegularExpressionMatch match = regex.match(matches);
            if (match.hasMatch() && !match.captured(1).isEmpty()) {
                // Help for specific command
                QString commandName = match.captured(1);
                qDebug() << getCommandHelp(commandName);
            } else {
                // List all commands
                qDebug() << "Available commands:";
                QStringList commands = getAvailableCommands();
                for (const QString& cmd : commands) {
                    qDebug() << "  " << cmd;
                }
            } }, "Scene", "Show help for commands", "help [command_name]");
}

void CommandManager::registerAllDefaultCommands()
{
    registerBasicCommands();
    registerMathCommands();
    registerAnimationCommands();
    registerSceneCommands();
}
