#include "parser.h"
#include <QDebug>

Parser::Parser()
{
    m_parser = new mu::Parser();
    setupBuiltinFunctions();
}

Parser::~Parser()
{
    clearVariables();
    delete m_parser;
}

double Parser::evaluate(const QString &expression)
{
    try
    {
        m_parser->SetExpr(expression.toStdWString());
        return m_parser->Eval();
    }
    catch (mu::Parser::exception_type &e)
    {
        qDebug() << "Parser error:" << QString::fromStdWString(e.GetMsg());
        return 0.0;
    }
}

void Parser::setVariable(const QString &name, double value)
{
    if (!m_variables.contains(name))
    {
        m_variables[name] = new double(value);
        m_parser->DefineVar(name.toStdWString(), m_variables[name]);
    }
    else
    {
        *m_variables[name] = value;
    }
}

double Parser::evaluateDynamic(const QString &expression, const QHash<QString, double> &trackerValues)
{
    // First expand the dynamic expression
    QString expandedExpression = expandDynamicExpression(expression);

    // Temporarily set tracker values as variables
    QHash<QString, double *> tempVariables;
    for (auto it = trackerValues.begin(); it != trackerValues.end(); ++it)
    {
        const QString &name = it.key();
        double value = it.value();

        if (!m_variables.contains(name))
        {
            tempVariables[name] = new double(value);
            m_parser->DefineVar(name.toStdWString(), tempVariables[name]);
        }
        else
        {
            *m_variables[name] = value;
        }
    }

    // Evaluate the expression
    double result = evaluate(expandedExpression);

    // Clean up temporary variables
    for (auto it = tempVariables.begin(); it != tempVariables.end(); ++it)
    {
        m_parser->RemoveVar(it.key().toStdWString());
        delete it.value();
    }

    return result;
}

void Parser::setVariables(const QHash<QString, double> &variables)
{
    for (auto it = variables.begin(); it != variables.end(); ++it)
    {
        setVariable(it.key(), it.value());
    }
}

QString Parser::expandDynamicExpression(const QString &expression)
{
    QString expanded = expression;

    // Replace [varname] with varname for muparser evaluation
    QRegularExpression bracketPattern(R"(\[(\w+)\])");
    QRegularExpressionMatchIterator iterator = bracketPattern.globalMatch(expression);

    // Process matches in reverse order to avoid position shifts
    QList<QRegularExpressionMatch> matches;
    while (iterator.hasNext())
    {
        matches.prepend(iterator.next());
    }

    for (const QRegularExpressionMatch &match : matches)
    {
        QString varName = match.captured(1);
        QString bracketedVar = match.captured(0); // [varname]

        // Replace [varname] with varname
        expanded.replace(match.capturedStart(0), match.capturedLength(0), varName);
    }

    return expanded;
}

double Parser::getVariable(const QString &name) const
{
    if (m_variables.contains(name))
    {
        return *m_variables[name];
    }
    return 0.0;
}

bool Parser::hasVariable(const QString &name) const
{
    return m_variables.contains(name);
}

void Parser::clearVariables()
{
    for (auto var : std::as_const(m_variables))
    {
        delete var;
    }
    m_variables.clear();
    m_parser->ClearVar();
}

bool Parser::isValidExpression(const QString &expression)
{
    try
    {
        m_parser->SetExpr(expression.toStdWString());
        m_parser->Eval();
        return true;
    }
    catch (mu::Parser::exception_type &)
    {
        return false;
    }
}

void Parser::setupBuiltinFunctions()
{
    // Built-in functions are already available in muParser
    // sin, cos, tan, sqrt, log, exp, etc.

    // You can add custom functions here if needed
    // m_parser->DefineFun("myFunc", myFunctionPtr, false);
}
