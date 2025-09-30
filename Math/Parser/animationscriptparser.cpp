#include "animationscriptparser.h"
#include <QRegularExpressionMatch>

AnimationScriptParser::AnimationScriptParser()
{
    initializeRegexPatterns();
}

QVector<TrackerCommand> AnimationScriptParser::parseScript(const QString &script)
{
    QVector<TrackerCommand> commands;

    // First, try to parse loops that might span multiple lines
    QRegularExpression loopPattern(R"(\s*loop\s*\(\s*(\w+)\s*:\s*(-?\d+)\s*->\s*(-?\d+)\s*\)\s*\{(.*?)\}\s*)",
                                   QRegularExpression::DotMatchesEverythingOption);

    QString remainingScript = script;
    QRegularExpressionMatchIterator loopIterator = loopPattern.globalMatch(script);

    // Process all loops first
    while (loopIterator.hasNext())
    {
        QRegularExpressionMatch loopMatch = loopIterator.next();
        QString iteratorName = loopMatch.captured(1);
        QString startStr = loopMatch.captured(2);
        QString endStr = loopMatch.captured(3);
        QString loopBody = loopMatch.captured(4);

        // Convert start and end to integers
        bool startOk, endOk;
        int startValue = startStr.toInt(&startOk);
        int endValue = endStr.toInt(&endOk);

        if (startOk && endOk)
        {
            TrackerCommand loopCommand(iteratorName, startValue, endValue, loopBody);
            if (loopCommand.isValid())
            {
                commands.append(loopCommand);
            }
        }

        // Remove the loop from remaining script to avoid processing it again
        remainingScript.replace(loopMatch.captured(0), "");
    }

    // Now process remaining lines normally
    QStringList lines = remainingScript.split("\n", Qt::SkipEmptyParts);

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
    // qDebug() << "parseLine: Processing line:" << line;

    // Try each pattern in order

    // Try dynamic value tracker first (more specific)
    // qDebug() << "parseLine: Trying parseDynamicValueTracker...";
    TrackerCommand cmd = parseDynamicValueTracker(line);
    if (cmd.isValid())
    {
        // qDebug() << "parseLine: SUCCESS with parseDynamicValueTracker, returning command with name:" << cmd.getName();
        return cmd;
    }

    // Try dynamic point tracker
    // qDebug() << "parseLine: Trying parseDynamicPointTracker...";
    cmd = parseDynamicPointTracker(line);
    if (cmd.isValid())
    {
        // qDebug() << "parseLine: SUCCESS with parseDynamicPointTracker, returning command with name:" << cmd.getName();
        return cmd;
    }

    // Try regular value tracker
    // qDebug() << "parseLine: Trying parseValueTracker...";
    cmd = parseValueTracker(line);
    if (cmd.isValid())
    {
        // qDebug() << "parseLine: SUCCESS with parseValueTracker, returning command with name:" << cmd.getName();
        return cmd;
    }

    // Try point tracker
    // qDebug() << "parseLine: Trying parsePointTracker...";
    cmd = parsePointTracker(line);
    if (cmd.isValid())
    {
        // qDebug() << "parseLine: SUCCESS with parsePointTracker, returning command with name:" << cmd.getName();
        return cmd;
    }

    // Try connection
    // qDebug() << "parseLine: Trying parseConnection...";
    cmd = parseConnection(line);
    if (cmd.isValid())
    {
        // qDebug() << "parseLine: SUCCESS with parseConnection, returning command with name:" << cmd.getName();
        return cmd;
    }

    // Try loop
    // qDebug() << "parseLine: Trying parseLoop...";
    cmd = parseLoop(line);
    if (cmd.isValid())
    {
        // qDebug() << "parseLine: SUCCESS with parseLoop, returning command with name:" << cmd.getName();
        return cmd;
    }

    // If nothing matched, return invalid command
    // qDebug() << "parseLine: NO MATCHES FOUND, returning invalid command";
    return TrackerCommand("", 0.0);
}
TrackerCommand AnimationScriptParser::parseValueTracker(const QString &line)
{
    // qDebug() << "parseValueTracker: Testing line:" << line;
    QRegularExpressionMatch match = m_reValExpr.match(line);
    if (match.hasMatch())
    {
        QString name = match.captured(1);
        QString expression = match.captured(2).trimmed();

        // qDebug() << "parseValueTracker: MATCH FOUND!";
        // qDebug() << "  - Full match:" << match.captured(0);
        // qDebug() << "  - Captured name (group 1):" << name;
        // qDebug() << "  - Captured expression (group 2):" << expression;

        if (isNumeric(expression))
        {
            // Simple numeric value
            double value = expression.toDouble();
            // qDebug() << "  - Creating numeric tracker:" << name << "=" << value;
            return TrackerCommand(name, value);
        }
        else
        {
            // Expression-based
            // qDebug() << "  - Creating expression tracker:" << name << "=" << expression;
            return TrackerCommand(name, expression, TrackerCommand::EXPRESSION_TRACKER);
        }
    }
    else
    {
        // qDebug() << "parseValueTracker: NO MATCH for line:" << line;
        // qDebug() << "  - Regex pattern:" << m_reValExpr.pattern();
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
    // qDebug() << "parseDynamicValueTracker: Testing line:" << line;
    QRegularExpressionMatch match = m_reDval.match(line);
    if (match.hasMatch())
    {
        QString name = match.captured(1);
        QString expression = match.captured(2).trimmed();

        // qDebug() << "parseDynamicValueTracker: MATCH FOUND!";
        // qDebug() << "  - Full match:" << match.captured(0);
        // qDebug() << "  - Captured name (group 1):" << name;
        // qDebug() << "  - Captured expression (group 2):" << expression;

        TrackerCommand cmd(name, expression, TrackerCommand::DYNAMIC_VALUE_TRACKER);
        QStringList deps = extractDependencies(expression);
        cmd.setDependencies(deps);
        return cmd;
    }
    else
    {
        // qDebug() << "parseDynamicValueTracker: NO MATCH for line:" << line;
        // qDebug() << "  - Regex pattern:" << m_reDval.pattern();
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

TrackerCommand AnimationScriptParser::parseLoop(const QString &line)
{
    QRegularExpressionMatch match = m_reLoop.match(line);
    if (match.hasMatch())
    {
        QString iteratorName = match.captured(1);
        QString startStr = match.captured(2);
        QString endStr = match.captured(3);
        QString loopBody = match.captured(4);

        // Convert start and end to integers
        bool startOk, endOk;
        int startValue = startStr.toInt(&startOk);
        int endValue = endStr.toInt(&endOk);

        // Validate conversion success
        if (!startOk || !endOk)
        {
            return TrackerCommand("", 0.0); // Invalid
        }

        // Additional validation is handled in TrackerCommand constructor:
        // - startValue < endValue
        // - (endValue - startValue) <= 50
        // - iteratorName not empty
        // - loopBody not empty

        return TrackerCommand(iteratorName, startValue, endValue, loopBody);
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
    // More flexible whitespace: \s* allows zero or more spaces everywhere
    m_reValExpr = QRegularExpression(R"(\s*val\s+(\w+)\s*=\s*([^;]+?)\s*;\s*)");

    // Pattern for pval name = (expr1, expr2); (catches both numeric and expression)
    // More flexible whitespace and non-greedy matching for expressions
    m_rePvalExpr = QRegularExpression(R"(\s*pval\s+(\w+)\s*=\s*\(\s*([^,]+?)\s*,\s*([^)]+?)\s*\)\s*;\s*)");

    // Pattern for dval name = [var1] + [var2] + expr;
    // More flexible whitespace
    m_reDval = QRegularExpression(R"(\s*dval\s+(\w+)\s*=\s*([^;]+?)\s*;\s*)");

    // Pattern for dpval name = ([var1] + expr, [var2] + expr);
    // More flexible whitespace
    m_reDpval = QRegularExpression(R"(\s*dpval\s+(\w+)\s*=\s*\(\s*([^,]+?)\s*,\s*([^)]+?)\s*\)\s*;\s*)");

    // Pattern for connect(tracker, object.property);
    // More flexible whitespace
    m_reConnect = QRegularExpression(R"(\s*connect\s*\(\s*(\w+)\s*,\s*(\w+)\s*\.\s*(\w+)\s*\)\s*;\s*)");

    // Pattern for loop (it_name:startnum->endnum){...}
    // Captures: iterator name, start number, end number, loop body
    m_reLoop = QRegularExpression(R"(\s*loop\s*\(\s*(\w+)\s*:\s*(-?\d+)\s*->\s*(-?\d+)\s*\)\s*\{\s*(.*?)\s*\}\s*)", QRegularExpression::DotMatchesEverythingOption);
}