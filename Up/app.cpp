#include "app.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QApplication>
#include "comun.h"
#include "manager.h"

App::App( QWidget *parent ) : Navegador( parent )
{
    this->init( 1, 4 );

    principal = new Principal( NULL );
    confEvento = new ConfEvento( NULL );
    search = new Search( NULL );
    publicador = new Publicador( NULL );

    this->setWidget( principal, 0, 0 );
    this->setWidget( confEvento, 0, 1 );
    this->setWidget( search, 0, 2 );
    this->setWidget( publicador, 0, 3 );

    this->setWindowTitle( APP_NAME );
    this->setWidgetActual( 0, 0 );

    this->principal->setTabFocus( false );
    this->confEvento->setTabFocus( false );
    this->search->setTabFocus( false );
    this->publicador->setTabFocus( false );


    connect( principal, SIGNAL( signal_crearEvento() ), this, SLOT( slot_crearEvento() ) );
    connect( principal, SIGNAL( signal_eventoExistente() ), this, SLOT( slot_eventoExistente() ) );
    connect( principal, SIGNAL( signal_proyectar() ), this, SLOT( slot_proyectar() ) );
    connect( principal, SIGNAL( signal_publicar() ), this, SLOT( slot_publicar() ) );
    connect( principal, SIGNAL( signal_salir() ), this, SLOT( close() ) );
    connect( confEvento, SIGNAL( signal_evento_nuevo() ), this, SLOT( slot_eventoNuevo() ) );
    connect( confEvento, SIGNAL( signal_evento_existente() ), this, SLOT( slot_evento_existente() ) );
    connect( confEvento, SIGNAL( signal_cancelar() ), this, SLOT( slot_abrirPrincipal() ) );
    connect( confEvento, SIGNAL( signal_confEvento_listoParaIniciarSearch() ),
             this, SLOT( slot_listoParaIniciarSearch() ) );
    connect( search, SIGNAL( signal_salirDelEvento() ), this, SLOT( slot_abrirPrincipal() ) );
    connect( publicador, SIGNAL( signal_volver() ), this, SLOT( slot_abrirPrincipal() ) );

    imFondo.load( ":/images/background3.jpg" );
}

App::~App()
{
//    Database::getInstance()->disconnect();
//    LOG_INF( "App: Destructor" );
}

Principal *App::getPrincipal() const
{
    return principal;
}

void App::setPrincipal(Principal *value)
{
    principal = value;
}


void App::paintEvent(QPaintEvent *)
{
    QPainter painter( this );

    painter.drawImage( 0, 0, imFondo.scaled( this->width(), this->height() ) );

}



// Para abrir ConfEvento en modo Crear evento, desde Principal
void App::slot_crearEvento()
{
    animacionRightKey( filaWidgetActual, columnaWidgetActual, 1 );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );

    this->principal->setTabFocus( false );
    this->confEvento->setTabFocus( true );
    this->search->setTabFocus( false );
    this->publicador->setTabFocus( false );

    confEvento->configurarWidgetPara( Comun::NUEVO );

    // Lo llamamos aca por si el usuario agrego plantillas mientras la aplicacion estaba abierta
    confEvento->cargarPlantillaDeDisco();

    // Lo llamamos aca por si hay nuevos hashtags
    confEvento->cargarHastHagsEnComboBox();
}

// Para abrir ConfEvento en modo Evento existente, desde Pricnipal
void App::slot_eventoExistente()
{
    animacionRightKey( filaWidgetActual, columnaWidgetActual, 1 );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );

    this->principal->setTabFocus( false );
    this->confEvento->setTabFocus( true );
    this->search->setTabFocus( false );
    this->publicador->setTabFocus( false );

    // Como es false, entonces se configura para evento existente
    confEvento->configurarWidgetPara( Comun::EXISTENTE );

    // Lo llamamos aca por si el usuario agrego plantillas mientras la aplicacion estaba abierta
    confEvento->cargarPlantillaDeDisco();

    // Lo llamamos aca por si hay nuevos hashtags
    confEvento->cargarHastHagsEnComboBox();
}

// Para abrir Search desde ConfEvento en modo Evento existente
void App::slot_evento_existente()
{
    this->resize( this->width() * 2, this->height() );

    animacionRightKey( filaWidgetActual, columnaWidgetActual, 1 );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );

    this->principal->setTabFocus( false );
    this->confEvento->setTabFocus( true );
    this->search->setTabFocus( false );
    this->publicador->setTabFocus( false );

    Comun::getInstance()->setTipoEvento( Comun::EXISTENTE );
    // Aca tener en cuenta que en ConfEvento se setea Comun para online u offline segun el checkbox
}

// Para abrir ConfEvento en modo Para Proyectar, desde Principal
void App::slot_proyectar()
{
    animacionRightKey( filaWidgetActual, columnaWidgetActual, 1 );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );

    this->principal->setTabFocus( false );
    this->confEvento->setTabFocus( true );
    this->search->setTabFocus( false );
    this->publicador->setTabFocus( false );

    // Se configura para Proyectar
    confEvento->configurarWidgetPara( Comun::PROYECTAR );

    // Lo llamamos aca por si hay nuevos hashtags
    confEvento->cargarHastHagsEnComboBox();

}

// Para abrir Publicador desde Principal
void App::slot_publicar()
{
    this->resize( this->width() * 2, this->height() );

    animacionRightKey( filaWidgetActual, columnaWidgetActual, 3 );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 3 );

    this->principal->setTabFocus( false );
    this->confEvento->setTabFocus( true );
    this->search->setTabFocus( false );
    this->publicador->setTabFocus( true );

    publicador->cargarHastHagsEnComboBox();

}

void App::slot_listoParaIniciarSearch()
{
    search->iniciar();
}


// Para abrir Search desde ConfEvento en modo Evento nuevo
void App::slot_eventoNuevo()
{
    this->resize( this->width() * 2, this->height() );

    animacionRightKey( filaWidgetActual, columnaWidgetActual, 1 );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );

    this->principal->setTabFocus( false );
    this->confEvento->setTabFocus( false );
    this->search->setTabFocus( true );
    this->publicador->setTabFocus( false );

    Comun::getInstance()->setTipoEvento( Comun::NUEVO );
    Comun::getInstance()->setInternet( Comun::ONLINE );  // Si es nuevo siempre sera online
}

void App::slot_abrirPrincipal()
{
    // Estas lineas redimensionan al tamano original. Ya que se desea que Principal se muestre siempre igual
    QRect rec = QApplication::desktop()->screenGeometry();
    int anchoScreen = rec.width();
    int altoScreen = rec.height();
    int anchoDeseado = anchoScreen / 2.5f;
    int altoDeseado = 3 * altoScreen / 4;

    this->resize( anchoDeseado, altoDeseado );
    this->move( anchoDeseado / 3, altoScreen / 2 - altoDeseado / 2 );

    // Desplaza desde ConfEvento hasta Principal
    if ( filaWidgetActual == 0 && columnaWidgetActual == 1 )  {
        animacionLeftKey( filaWidgetActual, columnaWidgetActual, 1 );
        setWidgetActual( filaWidgetActual, columnaWidgetActual - 1 );
    }

    // Desplaza desde Search hasta Principal
    if ( filaWidgetActual == 0 && columnaWidgetActual == 2 )  {

        animacionLeftKey( filaWidgetActual, columnaWidgetActual, 2 );
        setWidgetActual( filaWidgetActual, columnaWidgetActual - 2 );
    }

    // Desplaza desde Publicador hasta Principal
    if ( filaWidgetActual == 0 && columnaWidgetActual == 3 )  {
        animacionLeftKey( filaWidgetActual, columnaWidgetActual, 3 );
        setWidgetActual( filaWidgetActual, columnaWidgetActual - 3 );
    }


}

void App::keyPressEvent( QKeyEvent *e )
{
    qDebug() << "keyPressEvent";
    switch ( e->key() )  {
    case Qt::Key_Enter:
    case Qt::Key_Space:
    case Qt::Key_Return:
        break;
    case Qt::Key_Left:
//        animacionLeftKey( filaWidgetActual, columnaWidgetActual, 1 );
//        setWidgetActual( filaWidgetActual, columnaWidgetActual - 1 );
        break;
    case Qt::Key_Right:
//        animacionLeftKey( filaWidgetActual, columnaWidgetActual, 1 );
//        setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );
        break;



    default:;
    }
}

void App::closeEvent(QCloseEvent * e)
{
    if ( Manager::getInstance()->getWebInstagram()->isVisible() )  {
        QMessageBox::information( this, "Cerrar Instagram", "Antes de poder cerrar la aplicación debe "
                                  "cerrar la ventana de navegación que busca las fotos en Instagram.");
        e->ignore();
    }

    if ( Manager::getInstance()->getScene()->isVisible() )  {
        QMessageBox::information( this, "Cerrar la Escena", "Antes de poder cerrar la aplicación debe "
                                  "cerrar la ventana donde se están proyectando las fotos.");
        e->ignore();
    }
}


