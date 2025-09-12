#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <QObject>

class AnimationManager : public QObject
{
    Q_OBJECT
public:
    explicit AnimationManager(QObject *parent = nullptr);

signals:
};

#endif // ANIMATIONMANAGER_H
