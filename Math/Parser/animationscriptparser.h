#ifndef ANIMATIONSCRIPTPARSER_H
#define ANIMATIONSCRIPTPARSER_H

#include <QString>
#include <QRegularExpression>
#include <QVector>
#include "trackercommand.h"

class AnimationScriptParser
{
public:
    AnimationScriptParser();

    // Parse animation script and return list of commands
    QVector<TrackerCommand> parseScript(const QString &script);

    // Parse individual line and return command (or invalid command if parsing fails)
    TrackerCommand parseLine(const QString &line);

    // Check if a string is numeric
    bool isNumeric(const QString &str);

    // Extract dependencies from dynamic expressions
    QStringList extractDependencies(const QString &expression);

    // Check if expression contains bracketed variables [varname]
    bool isDynamicExpression(const QString &expression);

private:
    // Regular expressions for different command types
    QRegularExpression m_reVal;      // val name = 5;
    QRegularExpression m_reValExpr;  // val name = sin(t*2);
    QRegularExpression m_rePval;     // pval name = (1,2);
    QRegularExpression m_rePvalExpr; // pval name = (sin(t), cos(t));
    QRegularExpression m_reDval;     // dval name = [val1] + [val2];
    QRegularExpression m_reDpval;    // dpval name = ([val1], [val2]);
    QRegularExpression m_reConnect;  // connect(tracker, obj.prop);

    // Helper methods
    TrackerCommand parseValueTracker(const QString &line);
    TrackerCommand parsePointTracker(const QString &line);
    TrackerCommand parseDynamicValueTracker(const QString &line);
    TrackerCommand parseDynamicPointTracker(const QString &line);
    TrackerCommand parseConnection(const QString &line);

    void initializeRegexPatterns();
};

#endif // ANIMATIONSCRIPTPARSER_H