// mobjectmap.cpp
#include "mobjectmap.h"
#include "Math/Mobjects/Text.h"
#include "Math/Mobjects/Circle.h"
#include "Math/Mobjects/Curve.h"
#include "Math/Mobjects/MRectangle.h"
#include "Math/Mobjects/line.h"
#include "Math/Mobjects/Dot.h"

QHash<QString, MobjectMap::Factory> MobjectMap::map;

void MobjectMap::init(Scene *canvas)
{
    if (!map.isEmpty())
        return;
    map["Line"] = [canvas]()
    { return new Line(canvas, canvas); };
    map["Text"] = [canvas]()
    { return new Text(canvas, canvas); };
    map["Curve"] = [canvas]()
    { return new Curve(canvas, canvas); };
    map["Rectangle"] = [canvas]()
    { return new MRectangle(canvas, canvas); };
    map["Polygon"] = [canvas]()
    { return new Polygon(canvas, canvas); };
    map["Circle"] = [canvas]()
    { return new Circle(canvas, canvas); };
    map["Dot"] = [canvas]()
    { return new Dot(canvas, canvas); };
}
