#ifndef MOBJECTMAP_H
#define MOBJECTMAP_H

#include "Math/Mobjects/Mobject.h"
#include <functional>


class MobjectMap
{
public:
    using Factory = std::function<Mobject*()>;

    static QMap<QString,Factory> map;
    static void init(Scene *canvas);
};

#endif
