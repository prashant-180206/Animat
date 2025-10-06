#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QRegularExpression>
#include <memory>
#include "command.h"

class TrackerManager;
class Scene;

class CommandManager : public QObject
{
    Q_OBJECT
public:
    explicit CommandManager(QObject *parent = nullptr);

    // Command registration and management
    void registerCommand(const QString &name, const QRegularExpression &regex, Command::CommandHandler handler);
    void registerCommand(const QString &name, const QString &regexPattern, Command::CommandHandler handler);
    void unregisterCommand(const QString &name);
    void clearAllCommands();

    // Command execution
    bool executeCommand(const QString &input, TrackerManager *trackerManager, Scene *scene);

    // Command querying
    bool hasCommand(const QString &name) const;
    QStringList getCommandNames() const;
    QStringList getAvailableCommands() const;

    // Command categories
    void registerBasicCommands();
    void registerMathCommands();
    void registerAnimationCommands();
    void registerSceneCommands();
    void registerAllDefaultCommands();

    // Utilities
    QString getCommandHelp(const QString &name) const;
    QStringList getCommandsInCategory(const QString &category) const;

signals:
    void commandRegistered(const QString &name);
    void commandUnregistered(const QString &name);
    void commandExecuted(const QString &name, const QString &input);
    void commandFailed(const QString &input, const QString &error);

private:
    struct CommandInfo
    {
        std::unique_ptr<Command> command;
        QString category;
        QString description;
        QString usage;

        CommandInfo(std::unique_ptr<Command> cmd, const QString &cat = "", const QString &desc = "", const QString &use = "")
            : command(std::move(cmd)), category(cat), description(desc), usage(use) {}
    };

    QHash<QString, std::unique_ptr<CommandInfo>> m_commands;

    // Helper methods
    Command *findMatchingCommand(const QString &input, QString &matches) const;
    void registerCommandWithInfo(const QString &name, const QRegularExpression &regex,
                                 Command::CommandHandler handler, const QString &category,
                                 const QString &description, const QString &usage);
};

#endif // COMMANDMANAGER_H
