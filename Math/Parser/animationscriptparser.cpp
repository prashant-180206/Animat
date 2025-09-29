#include "animationscriptparser.h"
#include <QRegularExpressionMatch>

AnimationScriptParser::AnimationScriptParser()
{
    initializeRegexPatterns();
}

QVector<TrackerCommand> AnimationScriptParser::parseScript(const QString &script)
{
    QVector<TrackerCommand> commands;
    QStringList lines = script.split("\n", Qt::SkipEmptyParts);

    for (const QString &line : lines)
    {
        QString trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty() || trimmedLine.startsWith("//"))
        {
            continue; // Skip empty lines and comments
        }

        TrackerCommand command = parseLine(trimmedLine);
        if (command.isValid())
        {
            commands.append(command);
        }
    }

    return commands;
}

TrackerCommand AnimationScriptParser::parseLine(const QString &line)
{
    // Try each pattern in order

    // Try dynamic value tracker first (more specific)
    TrackerCommand cmd = parseDynamicValueTracker(line);
    if (cmd.isValid())
        return cmd;

    // Try dynamic point tracker
    cmd = parseDynamicPointTracker(line);
    if (cmd.isValid())
        return cmd;

    // Try regular value tracker
    cmd = parseValueTracker(line);
    if (cmd.isValid())
        return cmd;

    // Try point tracker
    cmd = parsePointTracker(line);
    if (cmd.isValid())
        return cmd;

    // Try connection
    cmd = parseConnection(line);
    if (cmd.isValid())
        return cmd;

    // If nothing matched, return invalid command
    return TrackerCommand("", 0.0);
}

TrackerCommand AnimationScriptParser::parseValueTracker(const QString &line)
{
    QRegularExpressionMatch match = m_reValExpr.match(line);
    if (match.hasMatch())
    {
        QString name = match.captured(1);
        QString expression = match.captured(2).trimmed();

        if (isNumeric(expression))
        {
            // Simple numeric value
            double value = expression.toDouble();
            return TrackerCommand(name, value);
        }
        else
        {
            // Expression-based
            return TrackerCommand(name, expression, TrackerCommand::EXPRESSION_TRACKER);
        }
    }

    return TrackerCommand("", 0.0); // Invalid
}

TrackerCommand AnimationScriptParser::parsePointTracker(const QString &line)
{
    QRegularExpressionMatch match = m_rePvalExpr.match(line);
    if (match.hasMatch())
    {
        QString name = match.captured(1);
        QString exprX = match.captured(2).trimmed();
        QString exprY = match.captured(3).trimmed();

        if (isNumeric(exprX) && isNumeric(exprY))
        {
            // Simple numeric point
            QPointF point(exprX.toDouble(), exprY.toDouble());
            return TrackerCommand(name, point);
        }
        else
        {
            // Expression-based point
            return TrackerCommand(name, exprX, exprY, TrackerCommand::POINT_EXPRESSION_TRACKER);
        }
    }

    return TrackerCommand("", 0.0); // Invalid
}

TrackerCommand AnimationScriptParser::parseDynamicValueTracker(const QString &line)
{
    QRegularExpressionMatch match = m_reDval.match(line);
    if (match.hasMatch())
    {
        QString name = match.captured(1);
        QString expression = match.captured(2).trimmed();

        TrackerCommand cmd(name, expression, TrackerCommand::DYNAMIC_VALUE_TRACKER);
        QStringList deps = extractDependencies(expression);
        cmd.setDependencies(deps);
        return cmd;
    }

    return TrackerCommand("", 0.0); // Invalid
}

TrackerCommand AnimationScriptParser::parseDynamicPointTracker(const QString &line)
{
    QRegularExpressionMatch match = m_reDpval.match(line);
    if (match.hasMatch())
    {
        QString name = match.captured(1);
        QString exprX = match.captured(2).trimmed();
        QString exprY = match.captured(3).trimmed();

        TrackerCommand cmd(name, exprX, exprY, TrackerCommand::DYNAMIC_POINT_TRACKER);
        QStringList depsX = extractDependencies(exprX);
        QStringList depsY = extractDependencies(exprY);
        QStringList allDeps = depsX + depsY;
        allDeps.removeDuplicates();
        cmd.setDependencies(allDeps);
        return cmd;
    }

    return TrackerCommand("", 0.0); // Invalid
}

TrackerCommand AnimationScriptParser::parseConnection(const QString &line)
{
    QRegularExpressionMatch match = m_reConnect.match(line);
    if (match.hasMatch())
    {
        QString trackerName = match.captured(1);
        QString objectName = match.captured(2);
        QString propertyName = match.captured(3);

        return TrackerCommand(trackerName, objectName, propertyName);
    }

    return TrackerCommand("", 0.0); // Invalid
}

bool AnimationScriptParser::isNumeric(const QString &str)
{
    bool ok;
    str.toDouble(&ok);
    return ok;
}

QStringList AnimationScriptParser::extractDependencies(const QString &expression)
{
    QStringList dependencies;
    QRegularExpression bracketPattern(R"(\[(\w+)\])");
    QRegularExpressionMatchIterator iterator = bracketPattern.globalMatch(expression);

    while (iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        QString varName = match.captured(1);
        if (!dependencies.contains(varName))
        {
            dependencies.append(varName);
        }
    }

    return dependencies;
}

bool AnimationScriptParser::isDynamicExpression(const QString &expression)
{
    QRegularExpression bracketPattern(R"(\[(\w+)\])");
    return expression.contains(bracketPattern);
}

void AnimationScriptParser::initializeRegexPatterns()
{
    // Pattern for val name = expression; (catches both numeric and expression)
    m_reValExpr = QRegularExpression(R"(val\s+(\w+)\s*=\s*([^;]+);)");

    // Pattern for pval name = (expr1, expr2); (catches both numeric and expression)
    m_rePvalExpr = QRegularExpression(R"(pval\s+(\w+)\s*=\s*\(\s*([^,]+)\s*,\s*([^)]+)\s*\);)");

    // Pattern for dval name = [var1] + [var2] + expr;
    m_reDval = QRegularExpression(R"(dval\s+(\w+)\s*=\s*([^;]+);)");

    // Pattern for dpval name = ([var1] + expr, [var2] + expr);
    m_reDpval = QRegularExpression(R"(dpval\s+(\w+)\s*=\s*\(\s*([^,]+)\s*,\s*([^)]+)\s*\);)");

    // Pattern for connect(tracker, object.property);
    m_reConnect = QRegularExpression(R"(connect\s*\(\s*(\w+)\s*,\s*(\w+)\.(\w+)\s*\);)");
}