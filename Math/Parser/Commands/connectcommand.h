#ifndef CONNECTCOMMAND_H
#define CONNECTCOMMAND_H

#include "command.h"
#include "Scene.h"
#include "funcmap.h"

#include <QDebug>

class connectcommand : public Command
{
public:
    connectcommand(QString line) : Command(line)
    {
        setRegex(QRegularExpression(R"(connectval\s*\(\s*([A-Za-z_]\w*)\s*,\s*([A-Za-z_]\w*)\.([A-Za-z_]\w*)\s*\)\s*;)"));
    }

    void execute(Scene *c, TrackerManager *tm) const override
    {
        QRegularExpressionMatch match = regex().match(line());
        if (match.hasMatch())
        {
            QString sourceTrackerName = match.captured(1);
            QString mobjectName = match.captured(2);
            QString propertyName = match.captured(3);

            auto m = c->getMobject(mobjectName);
            auto t = tm->getValueTracker(sourceTrackerName);

            if (m && t && FuncMap::ConnectFunc.contains(propertyName))
            {
                auto func = FuncMap::ConnectFunc[propertyName];
                func(m, t->value());
                QObject::connect(t, &ValueTracker::valueChanged, m, [m, func](qreal newval)
                                 { func(m, newval); });
            }

            auto pt = tm->getPtValueTracker(sourceTrackerName);
            if (m && pt && FuncMap::PtConnectFunc.contains(propertyName))
            {
                auto func = FuncMap::PtConnectFunc[propertyName];
                func(m, pt->value());
                QObject::connect(pt, &PtValueTracker::valueChanged, m, [m, func](QPointF newval)
                                 { func(m, newval); });
            }
        }
    }
};

#endif // CONNECTCOMMAND_H
