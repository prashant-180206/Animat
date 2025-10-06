#ifndef DPVALCOMMAND_H
#define DPVALCOMMAND_H
#include "command.h"
#include "muParser.h"
#include "trackermanager.h"
#include "valuetracker.h"

#include <QDebug>
#define wCMD_DEBUG(msg) qDebug() << "[dpvalcommand]" << msg;

// Helper: Replaces substrings using a map of replacements
inline QString multiReplace(QString expr, const QMap<QString, QString> &rep)
{
    for (auto it = rep.begin(); it != rep.end(); ++it)
    {
        expr.replace(it.key(), it.value());
    }
    return expr;
}

class dpvalcommand : public Command
{
public:
    dpvalcommand(QString line) : Command(line)
    {
        setRegex(QRegularExpression(R"(dpval\s+([A-Za-z_]\w*)\s*=\s*(.+);)"));
        // CMD_DEBUG("Constructor called");
    }
    void execute(Scene *c, TrackerManager *tm) const override
    {
        QRegularExpressionMatch match = regex().match(line());
        if (match.hasMatch())
        {
            QString ptname = match.captured(1);
            QString expression = match.captured(2);

            // 1. Find all value tracker dependencies ([val])
            QRegularExpression valRegex(R"(\[([A-Za-z_]\w*)\])");
            QRegularExpressionMatchIterator vi = valRegex.globalMatch(expression);
            QStringList valDeps;
            while (vi.hasNext())
                valDeps << vi.next().captured(1);

            // 2. Find all point tracker dependencies ({pt})
            QRegularExpression ptRegex(R"(\{([A-Za-z_]\w*)\})");
            QRegularExpressionMatchIterator pi = ptRegex.globalMatch(expression);
            QStringList ptDeps;
            while (pi.hasNext())
                ptDeps << pi.next().captured(1);

            // 3. Create variable placeholders for muparser: each point {pn} becomes pn_x and pn_y
            QMap<QString, double> valueMap;
            QMap<QString, QPointF> pointMap;

            for (const QString &d : std::as_const(valDeps))
                valueMap[d] = tm->hasValueTracker(d) ? tm->getValueTracker(d)->value() : 0;
            for (const QString &p : std::as_const(ptDeps))
                pointMap[p] = tm->hasPointTracker(p) ? tm->getPtValueTracker(p)->value() : QPointF();

            // 4. Build distinct expressions for x and y coordinates
            // Replace {p1} with (p1_x) and (p1_y) in respective expressions
            QMap<QString, QString> pt_x, pt_y;
            for (const QString &p : std::as_const(ptDeps))
            {
                pt_x["{" + p + "}"] = QString("(%1_x)").arg(p);
                pt_y["{" + p + "}"] = QString("(%1_y)").arg(p);
            }
            QString expr_x = expression;
            QString expr_y = expression;
            for (auto it = pt_x.begin(); it != pt_x.end(); ++it)
                expr_x.replace(it.key(), it.value());
            for (auto it = pt_y.begin(); it != pt_y.end(); ++it)
                expr_y.replace(it.key(), it.value());

            // 5. Setup muparser variables: must store doubles in local scope and pass pointers!
            QMap<QString, double> muVars;
            // Scalars
            for (const QString &d : std::as_const(valDeps))
                muVars[d] = valueMap[d];
            // Points
            for (const QString &p : std::as_const(ptDeps))
            {
                muVars[p + "_x"] = pointMap[p].x();
                muVars[p + "_y"] = pointMap[p].y();
            }

            mu::Parser px, py;
            for (auto it = muVars.begin(); it != muVars.end(); ++it)
            {
                px.DefineVar(it.key().toStdWString(), &(muVars[it.key()]));
                py.DefineVar(it.key().toStdWString(), &(muVars[it.key()]));
            }
            px.SetExpr(expr_x.toStdWString());
            py.SetExpr(expr_y.toStdWString());

            double x = px.Eval();
            double y = py.Eval();
            auto r = tm->createValueTracker(ptname, QPointF(x, y));

            // 6. Lambda for updating on change
            auto updateFunc = [tm, ptDeps, valDeps, r, expr_x, expr_y]()
            {
                QMap<QString, double> muVars;
                for (const QString &d : valDeps)
                    muVars[d] = tm->hasValueTracker(d) ? tm->getValueTracker(d)->value() : 0;
                for (const QString &p : ptDeps)
                {
                    QPointF pt = tm->hasPointTracker(p) ? tm->getPtValueTracker(p)->value() : QPointF();
                    muVars[p + "_x"] = pt.x();
                    muVars[p + "_y"] = pt.y();
                }
                mu::Parser px, py;
                for (auto it = muVars.begin(); it != muVars.end(); ++it)
                {
                    px.DefineVar(it.key().toStdWString(), &(muVars[it.key()]));
                    py.DefineVar(it.key().toStdWString(), &(muVars[it.key()]));
                }
                px.SetExpr(expr_x.toStdWString());
                py.SetExpr(expr_y.toStdWString());
                double x = px.Eval();
                double y = py.Eval();
                r->setValue(QPointF(x, y));
            };

            // 7. Connect all dependencies
            for (const QString &d : std::as_const(valDeps))
            {
                if (tm->hasValueTracker(d))
                    QObject::connect(tm->getValueTracker(d), &ValueTracker::valueChanged, r, [updateFunc](qreal)
                                     { updateFunc(); });
            }
            for (const QString &p : std::as_const(ptDeps))
            {
                if (tm->hasPointTracker(p))
                    QObject::connect(tm->getPtValueTracker(p), &PtValueTracker::valueChanged, r, [updateFunc](QPointF)
                                     { updateFunc(); });
            }
        }
        else
        {
            qDebug() << "No match for dpvalcommand with line:" << line();
        }
    }
};

#endif // DPVALCOMMAND_H
