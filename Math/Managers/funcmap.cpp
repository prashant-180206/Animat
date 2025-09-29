#include "funcmap.h"

QHash<QString, FuncMap::ValFunc> FuncMap::ConnectFunc = {
    {"height",
     [](ClickableMobject *m, qreal v)
     {
         if (!m->getProperties()->base())
             return;
         auto oldprop = m->getProperties()->base()->size();
         m->getProperties()->base()->setSize({v, oldprop.y()});
     }},
    {"width",
     [](ClickableMobject *m, qreal v)
     {
         if (!m->getProperties()->base())
             return;
         auto oldprop = m->getProperties()->base()->size();
         m->getProperties()->base()->setSize({oldprop.x(), v});
     }},
    {"radius",
     [](ClickableMobject *m, qreal v)
     {
         if (!m->getProperties()->circle())
             return;
         m->getProperties()->circle()->setRadius(v);
     }},
    {"opacity",
     [](ClickableMobject *m, qreal v)
     {
         if (!m->getProperties()->base())
             return;
         m->getProperties()->base()->setOpacity(v);
     }},
    {"thickness",
     [](ClickableMobject *m, qreal v)
     {
         if (m->getProperties()->line())
         {
             m->getProperties()->line()->setThickness(v);
         }
         if (m->getProperties()->polygon())
         {
             m->getProperties()->polygon()->setThickness(v);
         }
         return;
     }},
    {"tend",
     [](ClickableMobject *m, qreal v)
     {
         if (!m->getProperties()->curve())
             return;
         auto oldprop = m->getProperties()->curve()->tRange();
         m->getProperties()->curve()->setTRange({oldprop.x(), v});
     }},
};
;

QHash<QString, FuncMap::PtValFunc> FuncMap::PtConnectFunc = {
    {"position",
     [](ClickableMobject *m, QPointF v)
     {
         if (!m->getProperties()->base())
             return;
         m->getProperties()->base()->setPos(v);
     }},
    {"trange",
     [](ClickableMobject *m, QPointF v)
     {
         if (!m->getProperties()->curve())
             return;
         m->getProperties()->curve()->setTRange(v);
     }},
    {"lstart",
     [](ClickableMobject *m, QPointF v)
     {
         if (!m->getProperties()->line())
             return;
         m->getProperties()->line()->setLineStart(v);
     }},
    {"lend",
     [](ClickableMobject *m, QPointF v)
     {
         if (!m->getProperties()->line())
             return;
         m->getProperties()->line()->setLineEnd(v);
     }},

};
;

// Getter functions to retrieve current property values
QHash<QString, FuncMap::ValGetterFunc> FuncMap::GetterFunc = {
    {"height",
     [](ClickableMobject *m) -> qreal
     {
         if (!m->getProperties()->base())
             return 0.0;
         return m->getProperties()->base()->size().y();
     }},
    {"width",
     [](ClickableMobject *m) -> qreal
     {
         if (!m->getProperties()->base())
             return 0.0;
         return m->getProperties()->base()->size().x();
     }},
    {"radius",
     [](ClickableMobject *m) -> qreal
     {
         if (!m->getProperties()->circle())
             return 0.0;
         return m->getProperties()->circle()->radius();
     }},
    {"opacity",
     [](ClickableMobject *m) -> qreal
     {
         if (!m->getProperties()->base())
             return 1.0;
         return m->getProperties()->base()->opacity();
     }},
    {"thickness",
     [](ClickableMobject *m) -> qreal
     {
         if (m->getProperties()->line())
         {
             return m->getProperties()->line()->thickness();
         }
         if (m->getProperties()->polygon())
         {
             return m->getProperties()->polygon()->thickness();
         }
         return 1.0;
     }},
    {"tend",
     [](ClickableMobject *m) -> qreal
     {
         if (!m->getProperties()->curve())
             return 1.0;
         return m->getProperties()->curve()->tRange().y();
     }},
};

QHash<QString, FuncMap::PtValGetterFunc> FuncMap::PtGetterFunc = {
    {"position",
     [](ClickableMobject *m) -> QPointF
     {
         if (!m->getProperties()->base())
             return QPointF(0, 0);
         return m->getProperties()->base()->pos();
     }},
    {"trange",
     [](ClickableMobject *m) -> QPointF
     {
         if (!m->getProperties()->curve())
             return QPointF(0, 1);
         return m->getProperties()->curve()->tRange();
     }},
    {"lstart",
     [](ClickableMobject *m) -> QPointF
     {
         if (!m->getProperties()->line())
             return QPointF(0, 0);
         return m->getProperties()->line()->lineStart();
     }},
    {"lend",
     [](ClickableMobject *m) -> QPointF
     {
         if (!m->getProperties()->line())
             return QPointF(0, 0);
         return m->getProperties()->line()->lineEnd();
     }},
};

FuncMap::FuncMap()
{
}
