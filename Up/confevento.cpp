#include "confevento.h"
#include "ui_confevento.h"
#include "adminDB.h"
#include "manager.h"
#include <QMessageBox>
#include <QDebug>

ConfEvento::ConfEvento(QWidget *parent) : QDialog(parent), ui(new Ui::ConfEvento)  {
    ui->setupUi(this);

    // Lo invocamos solo para asegurarnos que en Comun este el estado actual de estos ComboBoxes
    this->slot_cambioComboBoxPapel();

    // Lo invocamos solo para asegurarnos que en Comun este el estado actual de estos ComboBoxes
    this->slot_cambioComboBoxImpresion();

    // Carga los hashtag existentes por si el usuario quiere seguir actualizando alguno de ellos
    QStringList hashtagList;
    hashtagList += "Elejir un HashTag";
    hashtagList += Comun::getInstance()->getHashTags();
    ui->cbHashtags->addItems( hashtagList );

    this->cargarPlantillaDeDisco();

    connect( ui->pbAceptar, SIGNAL( pressed() ), this, SLOT( slot_aceptar() ) );
    connect( ui->leHashtag, SIGNAL( returnPressed() ), this, SLOT( slot_aceptar() ) );
    connect( ui->pbCancelar, SIGNAL( pressed() ), this, SLOT( slot_cancelar() ) );

    connect(ui->leHashtag, SIGNAL(textChanged(QString)), this, SLOT(slot_cambioLineEditDetectado()));
    connect(ui->cbHashtags, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_cambioComboBoxDetectado()));

    connect( ui->cb15x10, SIGNAL( stateChanged(int) ), this, SLOT( slot_cambioComboBoxPapel() ) );
    connect( ui->cb21x15, SIGNAL( stateChanged(int) ), this, SLOT( slot_cambioComboBoxPapel() ) );

    connect( ui->cbPdf, SIGNAL( stateChanged(int) ), this, SLOT( slot_cambioComboBoxImpresion() ) );
    connect( ui->cbImprimir, SIGNAL( stateChanged(int) ), this, SLOT( slot_cambioComboBoxImpresion() ) );
    connect( ui->cbNiPdfNiPrinter, SIGNAL( stateChanged(int) ), this, SLOT( slot_cambioComboBoxImpresion() ) );

}

ConfEvento::~ConfEvento()  {
    delete ui;
}

/**
 * @brief ConfigEvento::slot_crearEvento Crea un evento y luego bloquea la creacion de nuevo. Se debera iniciar de
 * nuevo la aplicacion para poder hacer otro evento.
 */
void ConfEvento::slot_aceptar()  {

    Comun::getInstance()->setArchivoPlantilla( ui->cbPlantillas->currentText() );

    // Entra a este if cuando se ingreso a ConfEvento a traves del boton Evento existente de Principal.
    if ( Comun::getInstance()->getTipoEvento() == Comun::EXISTENTE )  {

        if ( ui->cbHashtags->currentIndex() == 0 )  {
            QMessageBox::information(this, "Elegir un HashTag", "Seleccione un HashTag de la lista.");
            return;
        }

        Comun::getInstance()->setHashTag( ui->cbHashtags->currentText() );

        emit signal_evento_existente();
    }
    // Porque se ingreso con boton Crear evento desde Principal
    else if ( Comun::getInstance()->getTipoEvento() == Comun::NUEVO )  {

        QString hashtag_lineedit = ui->leHashtag->text();  // Si le pone o no el numeral # es indistinto

        if ( hashtag_lineedit.isEmpty() )  {
            QMessageBox::information(this, "Elegir un HashTag", "Ingrese un HashTag nuevo.");
            return;
        }

        if ( hashtag_lineedit.size() == 1 && hashtag_lineedit.startsWith( "#" ) )  {
            QMessageBox::information(this, "Elija un HashTag válido", "Sólo continee el numeral #.");
            return;
        }

        // Si pone el # como primer caracter, es indiferente
        if ( hashtag_lineedit.startsWith( "#" ) )  {
            hashtag_lineedit.remove( 0, 1 );
        }

        if ( Comun::getInstance()->getHashTags().contains( hashtag_lineedit ) )  {
            QMessageBox::information(this, "HashTag existente", "El HashTag ingresado ya existe. Si el HashTag"
                                     " es el que desea, entonces presione cancelar y elija Evento existente.");
            return;
        }

        Comun::getInstance()->setHashTag( hashtag_lineedit );

        emit signal_evento_nuevo();
    }
    // Entra a este if cuando se ingreso a ConfEvento a traves del boton Proyectar desde Principal.
    else if ( Comun::getInstance()->getTipoEvento() == Comun::PROYECTAR )  {

            if ( ui->cbHashtags->currentIndex() == 0 )  {
                QMessageBox::information(this, "Elegir un HashTag", "Seleccione un HashTag de la lista.");
                return;
            }

            Comun::getInstance()->setHashTag( ui->cbHashtags->currentText() );

            Manager::getInstance()->mostrarProyector();
        }

    // Todos los siguientes seteos se hacen luego de asegurarnos que el usuario ingreso los datos correctos.

    if ( ui->cb15x10->isChecked() )
        Comun::getInstance()->setSizeHoja( Comun::SIZE15x10 );
    else
        Comun::getInstance()->setSizeHoja( Comun::SIZE21x15 );

    if ( ui->cbPdf->isChecked() )
        Comun::getInstance()->setImpresion( Comun::PDF );
    else if ( ui->cbImprimir->isChecked() )
        Comun::getInstance()->setImpresion( Comun::IMPRIMIR );
    else  {
        Comun::getInstance()->setImpresion( Comun::NOPDF_NOPRINTER );
    }

    if ( ui->cbBuscarEnInstagram->isChecked() )  {
        Comun::getInstance()->setInternet( Comun::ONLINE );
        qDebug() << "cbBuscarEnInstagram Comun::ONLINE";
    }
    else  {
        Comun::getInstance()->setInternet( Comun::OFFLINE );
        qDebug() << "cbBuscarEnInstagram Comun::OFFLINE";
    }

    emit signal_confEvento_listoParaIniciarSearch();
}

void ConfEvento::slot_cancelar()  {
    Comun::getInstance()->setHashTag( "" );

    // Si se presiona cancelar y estamos en PROYETAR, entonces cerramos scene por las dudas que aun siga abierto
    if ( Comun::getInstance()->getTipoEvento() == Comun::PROYECTAR )    {
        Manager::getInstance()->detenerProyector();
    }

    emit signal_cancelar();
}

void ConfEvento::slot_cambioComboBoxDetectado()  {
    if ( ui->cbHashtags->currentIndex() == 0 )  // retornamos porque el primer item es solo "Elejir un HashTag"
        return;
}

void ConfEvento::slot_cambioLineEditDetectado()  {

}

void ConfEvento::slot_cambioComboBoxPapel()
{
    if ( ui->cb15x10->isChecked() && ! ui->cb21x15->isChecked() )  {
        Comun::getInstance()->setSizeHoja( Comun::SIZE15x10 );
        return;
    }

    if ( ! ui->cb15x10->isChecked() && ui->cb21x15->isChecked() )  {
        Comun::getInstance()->setSizeHoja( Comun::SIZE21x15 );
        return;
    }

    QMessageBox::critical( this, "Problema en la configuración del evento", "Por favor! Comunicar "
                           "esto al desarrollador problema en la selección del tamaño del papel.");

}

void ConfEvento::slot_cambioComboBoxImpresion()
{

    if ( ui->cbPdf->isChecked() && ! ui->cbImprimir->isChecked() && ! ui->cbNiPdfNiPrinter->isChecked() )  {
        Comun::getInstance()->setImpresion( Comun::PDF );
        qDebug() << "PDF";
        return;
    }

    if ( ! ui->cbPdf->isChecked() && ui->cbImprimir->isChecked() && ! ui->cbNiPdfNiPrinter->isChecked() )  {
        Comun::getInstance()->setImpresion( Comun::IMPRIMIR );
        qDebug() << "IMPRIMIR";
        return;
    }

    if ( ! ui->cbPdf->isChecked() && ! ui->cbImprimir->isChecked() && ui->cbNiPdfNiPrinter->isChecked() )  {
        Comun::getInstance()->setImpresion( Comun::NOPDF_NOPRINTER );
        qDebug() << "NOPDF_NOPRINTER";
        return;
    }

    QMessageBox::critical( this, "Problema en la configuración del evento", "Por favor! Comunicar "
                           "esto al desarrollador problema en la selección de PDF o Imprimir.");

}

void ConfEvento::setTabFocus( bool focus )
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

/**
 * @brief ConfEvento::configurarWidgetParaEventoNuevo Este widgets ConfEvento sirve tanto para un evento
 * nuevo como para uno existente. Si el bool nuevo es true, entonces se ocultaran los lineedits para evento
 * existente y viceversa.
 * @param nuevo
 */
void ConfEvento::configurarWidgetPara( Comun::TipoEvento tipoEvento )
{
    if ( tipoEvento == Comun::NUEVO )  {
        ui->lHashtagExistentes->setEnabled( false );
        ui->cbHashtags->setEnabled( false );
        ui->leHashtag->setEnabled( true );
        ui->lHashtagNuevo->setEnabled( true );
        ui->cbBuscarEnInstagram->setEnabled( false );
        ui->cbBuscarEnInstagram->setChecked( true );
        ui->gbImprimir->setEnabled( true );
        ui->gbPapel->setEnabled( true );
        ui->cbPlantillas->setEnabled( true );
    }
    else if ( tipoEvento == Comun::EXISTENTE )  {
        ui->lHashtagExistentes->setEnabled( true );
        ui->cbHashtags->setEnabled( true );
        ui->leHashtag->setEnabled( false );
        ui->lHashtagNuevo->setEnabled( false );
        ui->cbBuscarEnInstagram->setEnabled( true );
        ui->gbImprimir->setEnabled( true );
        ui->gbPapel->setEnabled( true );
        ui->cbPlantillas->setEnabled( true );
    }
    else if ( tipoEvento == Comun::PROYECTAR )  {
        ui->lHashtagExistentes->setEnabled( true );
        ui->cbHashtags->setEnabled( true );
        ui->leHashtag->setEnabled( false );
        ui->lHashtagNuevo->setEnabled( false );
        ui->cbBuscarEnInstagram->setEnabled( false );
        ui->cbBuscarEnInstagram->setChecked( false );
        ui->gbImprimir->setEnabled( false );
        ui->gbPapel->setEnabled( false );
        ui->cbPlantillas->setEnabled( false );

    }
    else  {  // Este caso no se puede dar

    }
}

void ConfEvento::cargarPlantillaDeDisco()
{
    ui->cbPlantillas->clear();

    // Buscamos todas las imagenes de plantillas y las cargamos en el combo box
    QDir dir("../plantillas/");
    if ( dir.exists() )  {  // Si o si existe, pero por las dudas
        QStringList filters;
        filters << "*.jpg" << "*.png";

        QFileInfoList listaFileInfoList = dir.entryInfoList(filters);
        QStringList listaPlantillas;

        for (int i=0 ; i<listaFileInfoList.size() ; i++)  {
            listaPlantillas += listaFileInfoList.at(i).fileName();
        }
        ui->cbPlantillas->addItems(listaPlantillas);
    }
}

void ConfEvento::cargarHastHagsEnComboBox()
{
    ui->cbHashtags->clear();

    // Carga los hashtag existentes por si el usuario quiere seguir actualizando alguno de ellos
    QStringList hashtagList;
    hashtagList += "Elejir un HashTag";
    hashtagList += Comun::getInstance()->getHashTags();
    ui->cbHashtags->addItems( hashtagList );

}
