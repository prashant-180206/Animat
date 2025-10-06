#ifndef MAKEPTVALCOMMAND_H
#define MAKEPTVALCOMMAND_H

#include "command.h"
#include "trackermanager.h"

#include <QDebug>
// #define CMD_DEBUG(msg) qDebug() << "[makePtvalcommand]" << msg;

class makePtvalcommand : public Command
{
public:
    makePtvalcommand(const QString &name) : Command(name)
    {
        setRegex(QRegularExpression(R"(pval\s+([A-Za-z_]\w*)\s*=\s*\(\s*([+-]?\d*\.?\d+)\s*,\s*([+-]?\d*\.?\d+)\s*\);)"));
        // CMD_DEBUG("Constructor called");
    }

    void execute(Scene *c, TrackerManager *tm) const override
    {
        QRegularExpressionMatch match = regex().match(line());
        if (match.hasMatch())
        {
            QString name = match.captured(1);
            qreal x = match.captured(2).toDouble();
            qreal y = match.captured(3).toDouble();
            tm->createValueTracker(name, QPointF(x, y));
        }
        else
        {
            qDebug() << "No match for makePtvalcommand with line:" << line();
        }
    }
};

#endif // MAKEPTVALCOMMAND_H
