#ifndef MOBJECTMAP_H
#define MOBJECTMAP_H

#include "Math/Mobjects/Base/ClickableMobject.h"
#include <functional>

class MobjectMap
{
public:
    using Factory = std::function<ClickableMobject *()>;
    static QHash<QString, Factory> map;
    static void init(Scene *canvas);
};

#endif
