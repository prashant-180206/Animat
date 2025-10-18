// mobjectmap.cpp
#include "mobjectmap.h"
#include "Math/Mobjects/MText/MText.h"
#include "Math/Mobjects/Basic/Circle.h"
#include "Math/Mobjects/Complex/Curve.h"
#include "Math/Mobjects/Basic/MRectangle.h"
#include "Math/Mobjects/Basic/line.h"
#include "Math/Mobjects/Basic/Vector.h"
#include "Math/Mobjects/Basic/Dot.h"
#include "Math/Mobjects/Complex/MPolygon.h"
#include "Math/Mobjects/Complex/plane.h"
#include "Math/Mobjects/Geometric/Angle.h"
QHash<QString, MobjectMap::Factory> MobjectMap::map;

void MobjectMap::init(Scene *canvas)
{
    if (!map.isEmpty())
        return;
    map["Line"] = [canvas]()
    { return new Line(canvas, canvas); };
    map["Vector"] = [canvas]()
    { return new Vector(canvas, canvas); };
    map["Text"] = [canvas]()
    { return new MText(canvas, canvas); };
    map["Curve"] = [canvas]()
    { return new Curve(canvas, canvas); };
    map["Rectangle"] = [canvas]()
    { return new MRectangle(canvas, canvas); };
    map["MPolygon"] = [canvas]()
    { return new MPolygon(canvas, canvas); };
    map["Circle"] = [canvas]()
    { return new Circle(canvas, canvas); };
    map["Dot"] = [canvas]()
    { return new Dot(canvas, canvas); };
    map["Plane"] = [canvas]()
    { return new Plane(canvas, canvas); };
    map["Angle"] = [canvas]()
    { return new Angle(canvas, canvas); };
}
