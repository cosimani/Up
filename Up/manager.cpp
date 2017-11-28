#include "manager.h"
#include "adminDB.h"
#include "comun.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>

Manager *Manager::instance = NULL;

Manager::Manager(QObject *parent) : QObject(parent),
                                    webInstagram( new WebView ),
                                    scene( new Scene ),
                                    isSceneProyectada( false )
{


}

Manager *Manager::getInstance()
{
    if( !instance )
    {
        instance = new Manager();
    }
    return instance;
}

Manager::~Manager()
{
    delete instance;
}


WebView *Manager::getWebInstagram() const
{
    return webInstagram;
}

Scene *Manager::getScene() const
{
    return scene;
}


void Manager::mostrarProyector()  {

    if ( Comun::getInstance()->getHashTag().isEmpty() )  {
        return;
    }

    scene->iniciar();
    isSceneProyectada = true;

    QDesktopWidget * desktopWidget = QApplication::desktop();

    qDebug() << "CAntidad de monitores" << desktopWidget->screenCount();

    if ( desktopWidget->screenCount() == 1 )  {  // Entra aca cuando hay un solo monitor
        qDebug() << "No se detecta un proyector o segunda pantalla. Igualmente se va ha mostrar";

        scene->show();

    }
    else  {  // Aca cuando hay 2 monitores (o mas)

        qDebug() << "scene->windowHandle()->setScreen( qApp->screens()[ 1 ] )";

        // Estas tres lineas son necesarias para mostrarlo directamente en la segunda pantalla

//        // Lo mostramos aca solo para que el metodo windowHandle() devuelva que esta activo y luego poder
//        // setearle el QScreen donde queremos que se visualice en modo fullscreen
//        scene->show();

//        QWindow *window = scene->windowHandle();
//        qDebug() << "========== scene->windowHandle()"
//                 << window->isActive() << window->isExposed() << window->isTopLevel()
//                 << window->isVisible() << window->isExposed() << window->x() << window->y()
//                 << window->width() << window->height() << window->title();


//        scene->windowHandle()->setScreen( qApp->screens()[ 1 ] );
        
//        qApp->topLevelWindows();

        QList< QScreen * > listadoDeScreen = qApp->screens();

        int screenPrincipal = QApplication::desktop()->primaryScreen();

        qDebug() << "screenPrincipal" << screenPrincipal;
        QWindowList listaWindows = qApp->topLevelWindows();

        for ( int i=0 ; i<listaWindows.size() ; i++ )  {
            qDebug() << "Window" << i << "=" << listaWindows.at( i )->isActive()
                     << listaWindows.at( i )->isTopLevel() << listaWindows.at( i )->x()
                     << listaWindows.at( i )->y() << listaWindows.at( i )->isVisible();
        }


        QScreen * s;
        for ( int i=0 ; i<listadoDeScreen.size() ; i++ )  {
            s = listadoDeScreen.at( i );
            qDebug() << "Screen" << i << s->geometry();

        }

        if ( listadoDeScreen.size() == 2 &&
             listadoDeScreen.at( 0 )->geometry().x() == listadoDeScreen.at( 1 )->geometry().x() )  {
            qDebug() << "Las pantallas estan duplicadas. Debera separarlas en dos para poder proyectar";

            scene->show();
        }
        else if ( listadoDeScreen.size() >= 2 )  {
            scene->move( listadoDeScreen.at( 1 )->geometry().x(), listadoDeScreen.at( 1 )->geometry().y() );

            scene->showFullScreen();
    //        scene->show();
        }
    }
}

void Manager::detenerProyector()
{
    Manager::getInstance()->getScene()->hide();
    Manager::getInstance()->getScene()->detener();
    isSceneProyectada = false;
}

bool Manager::getIsSceneProyectada() const
{
    return isSceneProyectada;
}
