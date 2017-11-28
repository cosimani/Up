#include <QApplication>
#include <QNetworkProxyFactory>
#include <QWebSettings>
#include <QNetworkProxy>
#include <QDesktopWidget>
#include <QDebug>
#include <QRect>
#include <QSharedMemory>

#include "publicador.h"
#include "instagram.h"

#include "principal.h"

#include "adminDB.h"
#include "comun.h"
#include "app.h"

#include <QFontDatabase>

int main(int argc, char ** argv)  {
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fuentes/angelina.TTF");

    //---- Check for another instance code snippet ----

    // GUID : Generated once for your application
    // you could get one GUID here: http://www.guidgenerator.com/online-guid-generator.aspx
    QSharedMemory shared("1eecc105-3b51-4a05-ac38-a5d35abfff80");

    if( !shared.create( 512, QSharedMemory::ReadWrite) )  {
        // For a GUI application, replace this by :
        QMessageBox msgBox;
        msgBox.setText( "El programa ya estaba iniciado de antes." );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();

        return 0;
    }
    //---- END OF Check for another instance code snippet ----


    QRect screenres = QApplication::desktop()->screenGeometry(1/*screenNumber*/);
    qDebug() << QPoint(screenres.x(), screenres.y());
    qDebug() << screenres.width() << screenres.height();

//    QNetworkProxy proxy;
//    proxy.setType(QNetworkProxy::HttpProxy);
//    proxy.setHostName("192.168.6.253");
//    proxy.setPort(3128);
//    proxy.setUser("cosimani");
//    proxy.setPassword("c0simani");
//    QNetworkProxy::setApplicationProxy(proxy);

//    QNetworkProxyFactory::setUseSystemConfiguration(true);

    QWebSettings * settings = QWebSettings::globalSettings();
    settings->setFontFamily(QWebSettings::StandardFont, "Times New Roman");
    settings->setFontSize(QWebSettings::DefaultFontSize, 12);

    Comun::getInstance()->iniciar();

//    Manager * manager = Manager::getInstance();
//    manager->iniciar();

    // App
    App *app = new App( NULL );

    QRect rec = QApplication::desktop()->screenGeometry();
    int anchoScreen = rec.width();
    int altoScreen = rec.height();

    app->resize( anchoScreen / 2.5f, 3 * altoScreen / 4 );
    app->move( app->width() / 3, altoScreen / 2 - app->height() / 2 );
    app->show();


//    Instagram * instagram = new Instagram;
//    instagram->show();

//    Principal * principal = new Principal;
//    principal->move(0, 0);
//    principal->show();

    return a.exec();
}
