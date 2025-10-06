#ifndef DVALCOMMAND_H
#define DVALCOMMAND_H
#include "command.h"
#include "muParser.h"
#include "trackermanager.h"
#include "valuetracker.h"

#include <QDebug>
// #define CMD_DEBUG(msg) qDebug() << "[dvalcommand]" << msg;

class dvalcommand : public Command
{
public:
    dvalcommand(QString line) : Command(line)
    {
        setRegex(QRegularExpression(R"(dval\s+([A-Za-z_]\w*)\s*=\s*(.+);)"));
        // CMD_DEBUG("Constructor called");
    }
    void execute(Scene *c, TrackerManager *tm) const override
    {
        QRegularExpressionMatch match = regex().match(line());
        if (match.hasMatch())
        {
            QString valname = match.captured(1);
            QString expression = match.captured(2);

            QRegularExpression varNameRegex(R"(\[([A-Za-z_]\w*)\])");
            QRegularExpressionMatchIterator i = varNameRegex.globalMatch(expression);

            QStringList dependencies;
            QMap<QString, double> valueMap;
            while (i.hasNext())
            {
                QRegularExpressionMatch varMatch = i.next();
                QString varName = varMatch.captured(1);
                double value = tm->hasValueTracker(varName) ? tm->getValueTracker(varName)->value() : 0;
                valueMap[varName] = value;
                dependencies << varName;
            }

            // Replace [varname] in the expression with its corresponding value for initial evaluation
            QString replacedExpr = expression;
            for (auto it = valueMap.begin(); it != valueMap.end(); ++it)
            {
                QString bracketed = "[" + it.key() + "]";
                replacedExpr.replace(bracketed, QString::number(it.value()));
            }

            // muparser evaluation
            mu::Parser parser;
            parser.SetExpr(replacedExpr.toStdWString());
            double result = parser.Eval();
            auto r = tm->createValueTracker(valname, result);

            // Lambda to re-evaluate and update tracker
            auto updateFunc = [r, tm, expression, dependencies]()
            {
                QString expr = expression;
                // Replace each [dep] with its current value
                for (const QString &dep : dependencies)
                {
                    double val = tm->hasValueTracker(dep) ? tm->getValueTracker(dep)->value() : 0;
                    expr.replace("[" + dep + "]", QString::number(val));
                }
                mu::Parser parser;
                parser.SetExpr(expr.toStdWString());
                double v = parser.Eval();
                r->setValue(v);
            };

            // Connect every dependency's tracker to trigger updateFunc on value change
            for (const QString &dep : std::as_const(dependencies))
            {
                if (tm->hasValueTracker(dep))
                {
                    auto tracker = tm->getValueTracker(dep);
                    QObject::connect(tracker, &ValueTracker::valueChanged, r, [updateFunc](qreal)
                                     { updateFunc(); });
                }
            }
        }
        else
        {
            qDebug() << "No match for dvalcommand with line:" << line();
        }
    }
};

#endif // DVALCOMMAND_H
