#ifndef FUNCMAP_H
#define FUNCMAP_H

#include "Math/Mobjects/Base/ClickableMobject.h"
#include <QMap>
#include <QString>
#include <functional>

class FuncMap
{
public:
    FuncMap();

    using ValFunc = std::function<void(ClickableMobject *, qreal)>;
    using PtValFunc = std::function<void(ClickableMobject *, QPointF)>;

    // Getter functions to retrieve current values
    using ValGetterFunc = std::function<qreal(ClickableMobject *)>;
    using PtValGetterFunc = std::function<QPointF(ClickableMobject *)>;

    static QHash<QString, ValFunc> ConnectFunc;
    static QHash<QString, PtValFunc> PtConnectFunc;

    // Getter function maps
    static QHash<QString, ValGetterFunc> GetterFunc;
    static QHash<QString, PtValGetterFunc> PtGetterFunc;
};

#endif // FUNCMAP_H
