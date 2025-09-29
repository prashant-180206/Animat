#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QHash>
#include <QRegularExpression>
#include "../lib/include/muParser.h"

class Parser
{
public:
    Parser();
    ~Parser();

    // Evaluate mathematical expressions
    double evaluate(const QString &expression);

    // Evaluate dynamic expression by expanding [varname] brackets
    double evaluateDynamic(const QString &expression, const QHash<QString, double> &trackerValues);

    // Set variables that can be used in expressions
    void setVariable(const QString &name, double value);

    // Set multiple variables at once
    void setVariables(const QHash<QString, double> &variables);

    // Clear all variables
    void clearVariables();

    // Check if expression is valid
    bool isValidExpression(const QString &expression);

    // Expand dynamic expression: replace [varname] with varname
    QString expandDynamicExpression(const QString &expression);

    // Get current variable value
    double getVariable(const QString &name) const;

    // Check if variable exists
    bool hasVariable(const QString &name) const;

private:
    mu::Parser *m_parser;
    QHash<QString, double *> m_variables;

    void setupBuiltinFunctions();
};

#endif // PARSER_H
