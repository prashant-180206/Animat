#ifndef MOBJECTMAP_H
#define MOBJECTMAP_H

#include "Math/Helper/ClickableMobject.h"
#include <functional>


class MobjectMap
{
public:
    using Factory = std::function<ClickableMobject*()>;
    static QMap<QString,Factory> map;
    static void init(Scene *canvas);
};

#endif
