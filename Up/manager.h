#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "webview.h"
#include "scene.h"

class Manager : public QObject
{
    Q_OBJECT
public:
    static Manager *instance;
    explicit Manager( QObject *parent = 0 );
    ~Manager();

    static Manager *getInstance();

    WebView * getWebInstagram() const;

    Scene *getScene() const;

    void mostrarProyector();

    void detenerProyector();


    bool getIsSceneProyectada() const;

private:
    WebView * webInstagram;
    Scene * scene;
    bool isSceneProyectada;

signals:

public slots:
};

#endif // MANAGER_H
