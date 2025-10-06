#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <QMap>
#include <QRegularExpression>
#include <QString>
#include <memory>
#include "command.h"
#include "Commands/connectcommand.h"
#include "Commands/dpvalcommand.h"
#include "Commands/dvalcommand.h"
#include "Commands/makeptvalcommand.h"
#include "Commands/makevalcommand.h"

enum class CommandType
{
    ConnectCommand,
    DpvalCommand,
    DvalCommand,
    MakePtvalCommand,
    MakeValCommand,
    Unknown
};

class CommandFactory
{
public:
    CommandFactory()
    {
        initializeRegexMap();
    }

    // Get command type by checking regex patterns
    CommandType getCommandType(const QString &input) const
    {
        for (auto it = m_regexMap.constBegin(); it != m_regexMap.constEnd(); ++it)
        {
            if (it.value().match(input).hasMatch())
            {
                return it.key();
            }
        }
        return CommandType::Unknown;
    }

    // Create command instance based on type
    std::unique_ptr<Command> createCommand(CommandType type, const QString &input) const
    {
        switch (type)
        {
        case CommandType::ConnectCommand:
            return std::make_unique<connectcommand>(input);
        case CommandType::DpvalCommand:
            return std::make_unique<dpvalcommand>(input);
        case CommandType::DvalCommand:
            return std::make_unique<dvalcommand>(input);
        case CommandType::MakePtvalCommand:
            return std::make_unique<makePtvalcommand>(input);
        case CommandType::MakeValCommand:
            return std::make_unique<MakeValCommand>(input);
        default:
            return nullptr;
        }
    }

    // Convenience method to create command directly from input
    std::unique_ptr<Command> createCommand(const QString &input) const
    {
        CommandType type = getCommandType(input);
        return createCommand(type, input);
    }

    // Get all supported regex patterns for debugging
    QMap<CommandType, QRegularExpression> getRegexMap() const
    {
        return m_regexMap;
    }

private:
    QMap<CommandType, QRegularExpression> m_regexMap;

    void initializeRegexMap()
    {
        // Define regex patterns for each command type
        m_regexMap[CommandType::ConnectCommand] = QRegularExpression(
            R"(connectval\s*\(\s*([A-Za-z_]\w*)\s*,\s*([A-Za-z_]\w*)\.([A-Za-z_]\w*)\s*\)\s*;)");

        m_regexMap[CommandType::DpvalCommand] = QRegularExpression(
            R"(dpval\s+([A-Za-z_]\w*)\s*=\s*(.+);)");

        m_regexMap[CommandType::DvalCommand] = QRegularExpression(
            R"(dval\s+([A-Za-z_]\w*)\s*=\s*(.+);)");

        m_regexMap[CommandType::MakePtvalCommand] = QRegularExpression(
            R"(pval\s+([A-Za-z_]\w*)\s*=\s*\(\s*([+-]?\d*\.?\d+)\s*,\s*([+-]?\d*\.?\d+)\s*\);)");

        m_regexMap[CommandType::MakeValCommand] = QRegularExpression(
            R"(val\s+([A-Za-z_]\w*)\s*=\s*([+-]?\d*\.?\d+);)");
    }
};

#endif // COMMANDFACTORY_H