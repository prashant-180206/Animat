#ifndef FUNCMAP_H
#define FUNCMAP_H

#include "ClickableMobject.h"
#include <QMap>
#include <QString>
#include <functional>

class FuncMap
{
public:
    FuncMap() {
        ConnectFunc = {
                       {"height",
                           [](ClickableMobject* m , qreal v){
                               if(!m->getProperties()->base())  return;
                               auto oldprop =m->getProperties()->base()->size();
                               m->getProperties()->base()->setSize({v,oldprop.y()});
                           }
                       },
                       {"width",
                           [](ClickableMobject* m , qreal v){
                               if(!m->getProperties()->base())  return;
                               auto oldprop =m->getProperties()->base()->size();
                               m->getProperties()->base()->setSize({oldprop.x(),v});
                           }
                       },
                       {"radius",
                           [](ClickableMobject* m , qreal v){
                               if(!m->getProperties()->circle())  return;
                               m->getProperties()->circle()->setRadius(v);
                           }
                       },
                       {"opacity",
                           [](ClickableMobject* m , qreal v){
                               if(!m->getProperties()->base())  return;
                               m->getProperties()->base()->setOpacity(v);
                           }
                       },
                       {"thickness",
                           [](ClickableMobject* m , qreal v){
                               if(m->getProperties()->line()){
                                   m->getProperties()->line()->setThickness(v);
                               }
                               if(m->getProperties()->polygon()){
                                   m->getProperties()->polygon()->setThickness(v);
                               }
                               return ;
                           }
                       },
                       {"tend",
                           [](ClickableMobject* m , qreal v){
                               if(!m->getProperties()->curve()) return;
                               auto oldprop = m->getProperties()->curve()->tRange();
                               m->getProperties()->curve()->setTRange({oldprop.x(),v});
                           }
                       },
                       };

        PtConnectFunc = {
            {"position",
                [](ClickableMobject* m , QPointF v){
                    if(!m->getProperties()->base())  return;
                    m->getProperties()->base()->setPos(v);
                }
            },
            {"trange",
                [](ClickableMobject* m , QPointF v){
                    if(!m->getProperties()->curve())  return;
                    m->getProperties()->curve()->setTRange(v);
                }
            },
            {"lstart",
                [](ClickableMobject* m , QPointF v){
                    if(!m->getProperties()->line())  return;
                    m->getProperties()->line()->setLineStart(v);
                }
            },
            {"lend",
                [](ClickableMobject* m , QPointF v){
                    if(!m->getProperties()->line())  return;
                    m->getProperties()->line()->setLineEnd(v);
                }
            },

        };

    }

    using ValFunc = std::function<void(ClickableMobject*, qreal)>;
    using PtValFunc = std::function<void(ClickableMobject*, QPointF)>;

    static QHash<QString,ValFunc> ConnectFunc;
    static QHash<QString,PtValFunc> PtConnectFunc;
};

#endif // FUNCMAP_H
