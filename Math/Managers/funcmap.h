#ifndef FUNCMAP_H
#define FUNCMAP_H

#include "ClickableMobject.h"
#include <QMap>
#include <QString>
#include <functional>

class FuncMap
{
public:
    FuncMap();

    using ValFunc = std::function<void(ClickableMobject*, qreal)>;
    using PtValFunc = std::function<void(ClickableMobject*, QPointF)>;

    static QHash<QString,ValFunc> ConnectFunc;
    static QHash<QString,PtValFunc> PtConnectFunc;
};

#endif // FUNCMAP_H
