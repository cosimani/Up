#include "search.h"
#include "ui_search.h"
#include <QDebug>
#include "adminDB.h"
#include "comun.h"
#include <QMessageBox>
#include <QWindow>
#include <QKeyEvent>
#include <QWebFrame>
#include <QWebElement>
#include <QWebElementCollection>
#include "manager.h"

Search::Search(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Search),
                                  timerScrollInstagram( new QTimer( this ) ),
                                  timerUpdateInstagram( new QTimer( this ) )



{
    ui->setupUi(this);

    QIcon icon("../recursos/icon.jpg");
    this->setWindowIcon(icon);

    managerFoto = new QNetworkAccessManager(this);

    connect(managerFoto, SIGNAL( finished( QNetworkReply * ) ),
            this, SLOT( slot_descargaFotoFinalizada( QNetworkReply * ) ) );

    // timer para scroll.
    timerScrollInstagram->setInterval( Comun::getInstance()->getTimerScroll() );
    connect( timerScrollInstagram, SIGNAL( timeout() ), this, SLOT( slot_irFinalInstagram() ) );

    // timer para buscar de nuevo, a partir de que no se haga mas scroll
    timerUpdateInstagram->setInterval( Comun::getInstance()->getTimerActualizarPagina() );
    connect( timerUpdateInstagram, SIGNAL( timeout() ), this, SLOT( slot_updatePaginaInstagram() ) );

    ui->leTimerScroll->setText( QString::number( Comun::getInstance()->getTimerScroll() / 1000 ) );
    ui->leTimerWeb->setText( QString::number( Comun::getInstance()->getTimerActualizarPagina() / 1000 ) );
    ui->leTimerDir->setText( QString::number( Comun::getInstance()->getTimerDir() / 1000 ) );
    ui->leTimerImprimir->setText( QString::number( Comun::getInstance()->getTimerimpresiones() / 1000 ) );
    ui->leTimerEscena->setText( QString::number( Comun::getInstance()->getTimerEscena() / 1000 ) );

    qDebug() << "Constructor de Search - Timers:\n "
             << "getTimerScroll" << Comun::getInstance()->getTimerScroll()
             << "getTimerActualizarPagina" << Comun::getInstance()->getTimerActualizarPagina()
             << "getTimerDir" << Comun::getInstance()->getTimerDir()
             << "getTimerimpresiones" << Comun::getInstance()->getTimerimpresiones()
             << "getTimerEscena" << Comun::getInstance()->getTimerEscena();

    connect(ui->cbWebInstagram, SIGNAL(toggled(bool)), this, SLOT(slot_mostrarWebInstagram()));

    connect(ui->pbProyectar, SIGNAL(clicked()), this, SLOT(slot_mostrarProyectador()));

//    connect(ui->galeria, SIGNAL(signal_indexFotoChanged(int)), ui->impresor, SLOT(slot_setIndexFoto(int)));
    connect(ui->impresor, SIGNAL(signal_fotoEnGaleria(QString)), ui->galeria, SLOT(slot_fotoEnGaleria(QString)));

    connect(ui->impresor, SIGNAL(signal_fotoOriginalEnGaleria(QString)),
            ui->galeria, SLOT(slot_fotoOriginalEnGaleria(QString)));

    // Estos para actualizar timer en Comun cuando el usuario los modifica en la GUI
    connect(ui->leTimerScroll, SIGNAL(textChanged(QString)), this, SLOT(slot_actualizarTimerDesdeLineEdits()));
    connect(ui->leTimerWeb, SIGNAL(textChanged(QString)), this, SLOT(slot_actualizarTimerDesdeLineEdits()));
    connect(ui->leTimerDir, SIGNAL(textChanged(QString)), this, SLOT(slot_actualizarTimerDesdeLineEdits()));
    connect(ui->leTimerEscena, SIGNAL(textChanged(QString)), this, SLOT(slot_actualizarTimerDesdeLineEdits()));
    connect(ui->leTimerImprimir, SIGNAL(textChanged(QString)), this, SLOT(slot_actualizarTimerDesdeLineEdits()));

//    // Si por alguno motivo se setea algun timer, se llama a slot_recibeNotificacionTimersActualizados
//    connect( Comun::getInstance(), SIGNAL( signal_timerActualizado() ),
//             this, SLOT( slot_recibeNotificacionTimersActualizados() ) );

    connect( ui->pbSalir, SIGNAL( pressed() ), this, SLOT( slot_salirDelEvento() ) );

    connect( Manager::getInstance()->getWebInstagram(), SIGNAL( signal_webViewCerradoDesdeLaCruz() ),
             this, SLOT( slot_cerrarWebView() ) );

}

Search::~Search()  {
    delete ui;
}

void Search::slot_descargaFotoFinalizada(QNetworkReply *reply)  {
    QImage imFoto = QImage::fromData(reply->readAll());

    if ( ! imFoto.isNull() )  {
        imFoto.save("../fotos/" + Comun::getInstance()->getHashTag() +
                    "/" + reply->url().fileName());
        AdminDB::getInstance()->addFoto( Comun::getInstance()->getHashTag(), reply->url());
    }
    else
        qDebug() << "El QImage es null. Se llamo a slot_descargaFotoFinalizada pero no se puedo crear un QImage";
}

void Search::slot_mostrarProyectador()
{
    if ( Manager::getInstance()->getIsSceneProyectada() )  {
        Manager::getInstance()->detenerProyector();
        ui->pbProyectar->setText( "Proyectar" );
    }
    else  {
        Manager::getInstance()->mostrarProyector();
        ui->pbProyectar->setText( "Detener proyector" );
    }
}


void Search::iniciar()  {

    ui->leHashtagActual->setText( Comun::getInstance()->getHashTag() );

    // Solo se inician los timers para mostrar en la tablaWidget, que es Impresor, y si imprimie en PDF
    // o en la impresora o no imprime nada, lo decide Impresor.
    ui->impresor->iniciar();

    ui->impresor->setEnabled( true );

    if ( Comun::getInstance()->getTipoEvento() == Comun::NUEVO &&
         Comun::getInstance()->getInternet() == Comun::ONLINE )  {

        qDebug() << "Entro a Search en modo Comun::NUEVO y Comun::ONLINE";

        ui->cbWebInstagram->setChecked( false );  ui->cbWebInstagram->setEnabled( true );
        ui->leTimerImprimir->setEnabled( true );  ui->leTimerDir->setEnabled( true );
        ui->leTimerScroll->setEnabled( true );    ui->leTimerWeb->setEnabled( true );

        // Para que comience a buscar fotos.
        this->timerUpdateInstagram->start();

        connect( Manager::getInstance()->getWebInstagram(), SIGNAL( loadFinished( bool ) ),
                this, SLOT( slot_wvActualizadoInstagram( bool ) ) );

        // Tambien conectamos una sola vez para detectar cambios en la pagina
        QWebFrame * frameInstagram = Manager::getInstance()->getWebInstagram()->page()->mainFrame();
        connect( frameInstagram, SIGNAL(contentsSizeChanged(QSize)), this, SLOT(slot_contentChangeInstagram(QSize)));

        Manager::getInstance()->getWebInstagram()->load(QUrl("https://www.instagram.com/explore/tags/" +
                                                             Comun::getInstance()->getHashTag() ) );

        QDir dir("../fotos/" + Comun::getInstance()->getHashTag() );
        if ( ! dir.exists() )  {
            dir.setPath("../fotos/");
            dir.mkdir( Comun::getInstance()->getHashTag() );  // Creamos dir para la fotos
            dir.mkdir( Comun::getInstance()->getHashTag() + "_impresiones");  // Creamos dir para las impresiones
            dir.mkdir( Comun::getInstance()->getHashTag() + "_imagenes");  // Creamos dir para las imagenes
        }

        AdminDB::getInstance()->hashTagBuscado( Comun::getInstance()->getHashTag() );
    }
    else if ( Comun::getInstance()->getTipoEvento() == Comun::EXISTENTE  &&
              Comun::getInstance()->getInternet() == Comun::ONLINE )  {

        qDebug() << "Entro a Search en modo Comun::EXISTENTE y Comun::ONLINE";

        ui->cbWebInstagram->setChecked( false );  ui->cbWebInstagram->setEnabled( true );
        ui->leTimerImprimir->setEnabled( true );  ui->leTimerDir->setEnabled( true );
        ui->leTimerScroll->setEnabled( true );    ui->leTimerWeb->setEnabled( true );

        // Para que comience a buscar fotos.
        this->timerUpdateInstagram->start();

        connect( Manager::getInstance()->getWebInstagram(), SIGNAL( loadFinished( bool ) ),
                this, SLOT( slot_wvActualizadoInstagram( bool ) ) );

        // Tambien conectamos una sola vez para detectar cambios en la pagina
        QWebFrame * frameInstagram = Manager::getInstance()->getWebInstagram()->page()->mainFrame();
        connect( frameInstagram, SIGNAL( contentsSizeChanged( QSize ) ),
                 this, SLOT( slot_contentChangeInstagram( QSize ) ) );

        Manager::getInstance()->getWebInstagram()->load(QUrl("https://www.instagram.com/explore/tags/" +
                                                             Comun::getInstance()->getHashTag() ) );

        // El directorio ya deberia estar creado, por eso validamos asi.
        QDir dir("../fotos/" + Comun::getInstance()->getHashTag() );
        if ( ! dir.exists() )  {
            QMessageBox::critical( this, "Problema con directorio de esta fiesta",
                                   "Avisar al desarrollador por este problema.");

            // Cancela todo y emite la signal_salirDelEvento
            this->slot_salirDelEvento();

            return;
        }

        AdminDB::getInstance()->hashTagBuscado( Comun::getInstance()->getHashTag() );

    }
    else if ( Comun::getInstance()->getTipoEvento() == Comun::EXISTENTE  &&
              Comun::getInstance()->getInternet() == Comun::OFFLINE )  {

        qDebug() << "Entro a Search en modo Comun::EXISTENTE y Comun::OFFLINE";

        ui->cbWebInstagram->setChecked( false );   ui->cbWebInstagram->setEnabled( false );
        ui->leTimerImprimir->setEnabled( false );  ui->leTimerDir->setEnabled( false );
        ui->leTimerScroll->setEnabled( false );    ui->leTimerWeb->setEnabled( false );

        this->timerUpdateInstagram->stop();
        this->timerScrollInstagram->stop();

        if ( Comun::getInstance()->getImpresion() == Comun::PDF ||
             Comun::getInstance()->getImpresion() == Comun::IMPRIMIR )  {

            // Inicia las impresiones a PDF o en Impresora, en la clase Impresor se decide si es PDF o impresora
            ui->impresor->iniciar();
            ui->leTimerImprimir->setEnabled( true );  ui->leTimerDir->setEnabled( true );
        }
        else  {
            ui->impresor->cancelar();
        }

    }
    else  {  // No se da nunca esta combinacion, pero por las dudas ponemos todo offline

        qDebug() << "Entro a Search en modo DESCONOCIDO";

        ui->cbWebInstagram->setChecked( false );   ui->cbWebInstagram->setEnabled( false );
        ui->leTimerImprimir->setEnabled( false );  ui->leTimerDir->setEnabled( false );
        ui->leTimerScroll->setEnabled( false );    ui->leTimerWeb->setEnabled( false );

        this->timerUpdateInstagram->stop();
        this->timerScrollInstagram->stop();
        ui->impresor->cancelar();
    }

}

// Solo actualizamos la pagina
void Search::slot_updatePaginaInstagram()  {

    // Este lo usamos para ir 5 veces haciael final y hacia arriba. pasado esto, y si no hay cambios en el tamano
    // de la pagina, entonces detenemos el ir arriba y al final, para activar el timer de actualizacion de pagina
    cantidadDeCiclosIrFinalIrArriba = 5;

    Manager::getInstance()->getWebInstagram()->load(QUrl("https://www.instagram.com/explore/tags/" +
                                                         Comun::getInstance()->getHashTag() ) );
//    qDebug() << "SE carga en el wv la pagina de nueva";
}

// Instagram tiene un boton Cargar Mas para presionar y que muestre mas fotos, pero despues el boton ya no se debe
// presionar mas para que muestre mas fotos. Se debe hacer scroll al igual que twitter
void Search::slot_irFinalInstagram()  {

    // Primer hacemos scroll
    QPoint posicionAntesDeDesplazar = Manager::getInstance()->getWebInstagram()->page()->mainFrame()->scrollPosition();
//    qDebug() << posicionAntesDeDesplazar
//             << Manager::webInstagram->page()->mainFrame()->contentsSize()
//             << Manager::webInstagram->page()->viewportSize()
//             << Manager::webInstagram->page()->preferredContentsSize();

    int posicionYscroll = posicionAntesDeDesplazar.y();
    int altoVentanaVisible = Manager::getInstance()->getWebInstagram()->page()->viewportSize().height();
    int altoPaginaCompleta = Manager::getInstance()->getWebInstagram()->page()->mainFrame()->contentsSize().height();

    if ( posicionYscroll + altoVentanaVisible == altoPaginaCompleta )  {
        Manager::getInstance()->getWebInstagram()->page()->mainFrame()->setScrollPosition(QPoint(0, 0));
//        qDebug() << "posicionAntesDeDesplazar.y()" << posicionAntesDeDesplazar.y();

        cantidadDeCiclosIrFinalIrArriba--;
    }
    else  {
        Manager::getInstance()->getWebInstagram()->page()->mainFrame()->scroll(0, Manager::getInstance()->getWebInstagram()->page()->mainFrame()->contentsSize().height());
//        qDebug() << "Manager::webInstagram->page()->mainFrame()->contentsSize().height()" << Manager::webInstagram->page()->mainFrame()->contentsSize().height();
    }

    QWebFrame * frame = Manager::getInstance()->getWebInstagram()->page()->mainFrame();
    QWebElement document = frame->documentElement();

    // Debemos buscar el siguiente tag. Para darle clic y que pueda mostrar mas fotos
//    <a href="/explore/tags/ffaquilmescordoba/?max_id=1143158359878693784"> Cargar mas </a>

    QWebElementCollection coleccion = document.findAll("a");

    foreach (QWebElement elemento, coleccion) {
        QString elementoCargasMas;

        if ( elemento.attribute("href").contains("explore/tags") )  {  // Encontramos el boton Cargas Mas
            elementoCargasMas = elemento.attribute("href");

            elemento.evaluateJavaScript("this.click()");  // Le hacemos clic
        }
    }

    if ( cantidadDeCiclosIrFinalIrArriba <= 0 )  {
        timerScrollInstagram->stop();
        timerUpdateInstagram->start();
    }

}


void Search::slot_contentChangeInstagram(QSize)  {

    cuantasImagenesHastaAhoraInstagram = this->getImagenesInstagram().size();
//    qDebug() << "slot_contentChange Instagram - Se encontraron " + QString::number(cuantasImagenesHastaAhoraInstagram);

    timerScrollInstagram->start();  // Realiza scroll de la pagina cada tantos segundos
    timerUpdateInstagram->stop();

    cantidadDeCiclosIrFinalIrArriba = 5;
}


void Search::slot_wvActualizadoInstagram(bool exito)  {

    if ( ! exito )  {
        qDebug() << "No se cargo la pagina correctamente";
        return;
    }
    else  {
        qDebug() << "Exito en cargar la pagina de instagram - slot_wvActualizadoInstagram";
    }

    cuantasImagenesHastaAhoraInstagram = this->getImagenesInstagram().size();
//    qDebug() << " - Se encontraron " + QString::number(cuantasImagenesHastaAhoraInstagram);

    timerScrollInstagram->start();  // Realiza scroll de la pagina cada tantos segundos
}

QStringList Search::getImagenesInstagram()  {
    QNetworkRequest request;
    QStringList listaImagenes;

    QWebFrame * frame = Manager::getInstance()->getWebInstagram()->page()->mainFrame();

    QWebElement document = frame->documentElement();

    QWebElementCollection coleccion = document.findAll("img");

    foreach (QWebElement elemento, coleccion) {
        if ( elemento.attribute("alt").contains( Comun::getInstance()->getHashTag(),
                                                 Qt::CaseInsensitive) )  {
            listaImagenes.append(elemento.attribute("src"));
        }
    }

    // Si el archivo ya existe directamente ni pedimos a internet
    for (int i=0 ; i<listaImagenes.size() ; i++)  {

        QUrl url(listaImagenes.at(i));

        if ( ! this->isFile(url.fileName()) )  {
            request.setUrl(url);
            managerFoto->get(request);
//            qDebug() << "se pide instagram " << listaImagenes.at(i);
        }
    }

    return listaImagenes;
}

void Search::slot_mostrarWebInstagram()  {
    if ( ui->cbWebInstagram->isChecked() )
        Manager::getInstance()->getWebInstagram()->show();
    else
        Manager::getInstance()->getWebInstagram()->hide();
}



/**
 * @brief Search::slot_actualizarTimerDesdeLineEdits Se invoca cuando cambia el valor de alguno de los de QLineEdits
 */
void Search::slot_actualizarTimerDesdeLineEdits()  {

    Comun::getInstance()->setTimerActualizarPagina( ui->leTimerWeb->text().toInt() * 1000 );
    Comun::getInstance()->setTimerScroll( ui->leTimerScroll->text().toInt() * 1000 );
    Comun::getInstance()->setTimerDir( ui->leTimerDir->text().toInt() * 1000 );
    Comun::getInstance()->setTimerEscena( ui->leTimerEscena->text().toInt() * 1000 );
    Comun::getInstance()->setTimerimpresiones( ui->leTimerImprimir->text().toInt() * 1000 );

    timerUpdateInstagram->setInterval( Comun::getInstance()->getTimerActualizarPagina() );
    timerScrollInstagram->setInterval( Comun::getInstance()->getTimerScroll() );
    ui->impresor->setTimerDir( Comun::getInstance()->getTimerDir() );
    ui->impresor->setTimerImpresiones( Comun::getInstance()->getTimerimpresiones() );
    Manager::getInstance()->getScene()->setTimerNuevaImagen( Comun::getInstance()->getTimerEscena() );

    qDebug() << "slot_actualizarTimerDesdeLineEdits - Timers:\n "
             << "getTimerScroll" << Comun::getInstance()->getTimerScroll()
             << "getTimerActualizarPagina" << Comun::getInstance()->getTimerActualizarPagina()
             << "getTimerDir" << Comun::getInstance()->getTimerDir()
             << "getTimerimpresiones" << Comun::getInstance()->getTimerimpresiones()
             << "getTimerEscena" << Comun::getInstance()->getTimerEscena();


}

void Search::slot_salirDelEvento()
{
    // Detenemos todos los timers
    this->timerScrollInstagram->stop();
    this->timerUpdateInstagram->stop();
    ui->impresor->cancelar();

    // Desconectamos todo
    disconnect( Manager::getInstance()->getWebInstagram(), NULL, NULL, NULL );
    QWebFrame * frameInstagram = Manager::getInstance()->getWebInstagram()->page()->mainFrame();
    disconnect( frameInstagram, NULL, NULL, NULL );

    Manager::getInstance()->getWebInstagram()->stop();
    Manager::getInstance()->getWebInstagram()->hide();

    emit signal_salirDelEvento();
}

void Search::slot_cerrarWebView()
{
    ui->cbWebInstagram->setChecked( false );
}

void Search::slot_recibeNotificacionTimersActualizados()
{
    ui->leTimerScroll->setText( QString::number( Comun::getInstance()->getTimerScroll() / 1000 ) );
    ui->leTimerWeb->setText( QString::number( Comun::getInstance()->getTimerActualizarPagina() / 1000 ) );
    ui->leTimerDir->setText( QString::number( Comun::getInstance()->getTimerDir() / 1000 ) );
    ui->leTimerImprimir->setText( QString::number( Comun::getInstance()->getTimerimpresiones() / 1000 ) );
    ui->leTimerEscena->setText( QString::number( Comun::getInstance()->getTimerEscena() / 1000 ) );

    timerScrollInstagram->setInterval( Comun::getInstance()->getTimerScroll() );
    timerUpdateInstagram->setInterval( Comun::getInstance()->getTimerActualizarPagina() );
    ui->impresor->setTimerDir( Comun::getInstance()->getTimerDir() );
    ui->impresor->setTimerImpresiones( Comun::getInstance()->getTimerimpresiones() );
    Manager::getInstance()->getScene()->setTimerNuevaImagen( Comun::getInstance()->getTimerEscena() );

    qDebug() << "slot_recibeNotificacionTimersActualizados - Timers:\n "
             << "getTimerScroll" << Comun::getInstance()->getTimerScroll()
             << "getTimerActualizarPagina" << Comun::getInstance()->getTimerActualizarPagina()
             << "getTimerDir" << Comun::getInstance()->getTimerDir()
             << "getTimerimpresiones" << Comun::getInstance()->getTimerimpresiones()
             << "getTimerEscena" << Comun::getInstance()->getTimerEscena();

}

void Search::keyPressEvent(QKeyEvent *e)  {
    switch (e->key())  {
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Home:
    case Qt::Key_End:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        break;

    case Qt::Key_Left:
        qDebug() << "izquierda";
        break;
    case Qt::Key_Right:
        qDebug() << "derecha";
        break;

    default:;
    }
}

/**
 * @brief Search::showEvent Cada vez que esta ventana se visualice, preguntamos si esta seteado como
 * inline u off-line para configurar las opciones.
 */
void Search::showEvent( QShowEvent * )
{
    qDebug() << "void Search::showEvent( QShowEvent * )";
}


bool Search::isFile(QString archivo)  {
    QDir dir("../fotos/" + Comun::getInstance()->getHashTag() );
    QStringList filters;
    filters << "*.jpg" << "*.png";
    dir.setNameFilters(filters);
    QStringList list = dir.entryList();

    return list.contains(archivo, Qt::CaseInsensitive);
}

void Search::setTabFocus( bool focus )
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
