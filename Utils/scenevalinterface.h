#ifndef SCENEVALINTERFACE_H
#define SCENEVALINTERFACE_H

#include <QQuickItem>

class Scene;
class TrackerManager;

class SceneValInterface : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    struct TrackerData {
        QVariant minval;
        QVariant maxval;
        QVariant currentval;
        qreal viewtime;
    };


public:
    SceneValInterface();
    void extracted(QStringList &vals);
    void onFramechanged();
    ;

private:

    QMap<QString,TrackerData> active_trackers;

    QMap<QString,qreal> tracker_time;
    Scene * canvas;
    TrackerManager * tm;

signals:
};

#endif // SCENEVALINTERFACE_H
