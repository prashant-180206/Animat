#ifndef MAKEVALCOMMAND_H
#define MAKEVALCOMMAND_H

#include "command.h"
#include "trackermanager.h"

#include <QDebug>
#define rCMD_DEBUG(msg) qDebug() << "[MakeValCommand]" << msg;

class MakeValCommand : public Command
{
public:
    MakeValCommand(QString l) : Command(l)
    {
        setRegex(QRegularExpression(R"(val\s+([A-Za-z_]\w*)\s*=\s*([+-]?\d*\.?\d+);)"));
        // CMD_DEBUG("Constructor called");
    }

    void execute(Scene *c, TrackerManager *tm) const override
    {
        QRegularExpressionMatch match = regex().match(line());
        if (match.hasMatch())
        {
            QString valname = match.captured(1);
            QString value = match.captured(2);
            tm->createValueTracker(valname, value.toDouble());
        }
        else
        {
            qDebug() << "No match for MakeValCommand with line:" << line();
        }
    }
};

#endif // MAKEVALCOMMAND_H
