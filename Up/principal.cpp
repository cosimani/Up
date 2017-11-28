#include "principal.h"
#include "ui_principal.h"

#include "comun.h"

#include "adminDB.h"

#include "manager.h"


Principal::Principal(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::Principal)


{
    ui->setupUi(this);


    ui->background->setImage( "background3.jpg", FILL );

    ui->boton1->setColor( "#e91e63" );  ui->boton1->setTexto( "Crear evento" );
    ui->boton2->setColor( "#9c27b0" );  ui->boton2->setTexto( "Evento existente" );
    ui->boton3->setColor( "#673ab7" );  ui->boton3->setTexto( "Publicar álbum" );
    ui->boton4->setColor( "#3f51b5" );  ui->boton4->setTexto( "Proyectar" );
    ui->boton5->setColor( "#009688" );  ui->boton5->setTexto( "Salir" );

    connect(ui->boton1, SIGNAL(clic()), this, SLOT(slot_crearEvento()));
    connect(ui->boton2, SIGNAL(clic()), this, SLOT(slot_eventoExistente()));
    connect(ui->boton3, SIGNAL(clic()), this, SLOT(slot_publicar()));
    connect(ui->boton4, SIGNAL(clic()), this, SLOT(slot_proyectar()));
    connect(ui->boton5, SIGNAL(clic()), this, SLOT(slot_salir()));

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
    this->setPalette( palette );

}

Principal::~Principal()
{
    delete ui;
}

void Principal::resizeEvent( QResizeEvent* )
{
}

void Principal::slot_crearEvento()
{
    Comun::getInstance()->setTipoEvento( Comun::NUEVO );

    emit signal_crearEvento();
}

void Principal::slot_eventoExistente()
{
    Comun::getInstance()->setTipoEvento( Comun::EXISTENTE );

    emit signal_eventoExistente();
}


void Principal::slot_proyectar()
{
    Comun::getInstance()->setTipoEvento( Comun::PROYECTAR );

    emit signal_proyectar();
}

void Principal::slot_publicar()
{    
    // Para publicar detenemos los timers de actualizacion de intagram, facebook y twitter
//    this->timerScrollFacebook->stop();
//    this->timerScrollInstagram->stop();
//    this->timerScrollTwitter->stop();
//    this->timerUpdateFacebook->stop();
//    this->timerUpdateInstagram->stop();
//    this->timerUpdateTwitter->stop();

//    Manager::getInstance()->getPublicador()->setHashTag( Comun::getInstance()->getHashTag() );

//    Manager::getInstance()->getPublicador()->showMaximized();

    emit signal_publicar();
}

void Principal::slot_salir()
{
    emit signal_salir();
}

void Principal::setTabFocus( bool focus )
{
    if ( focus )  {
        this->setFocusPolicy( Qt::StrongFocus );
//        ui->welcomeLabel->setFocusPolicy( Qt::NoFocus );
//        ui->userLineEdit->setFocusPolicy( Qt::StrongFocus );
//        ui->passwordLineEdit->setFocusPolicy( Qt::StrongFocus );
//        ui->pbIngresar->setFocusPolicy( Qt::NoFocus );
//        ui->userLineEdit->setFocus();
    }
    else  {
        this->setFocusPolicy( Qt::NoFocus );
//        ui->welcomeLabel->setFocusPolicy( Qt::NoFocus );
//        ui->userLineEdit->setFocusPolicy( Qt::NoFocus );
//        ui->passwordLineEdit->setFocusPolicy( Qt::NoFocus );
//        ui->pbIngresar->setFocusPolicy( Qt::NoFocus );

    }
}



