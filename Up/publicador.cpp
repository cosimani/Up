#include "publicador.h"
#include "ui_publicador.h"
#include "adminDB.h"
#include "comun.h"

Publicador::Publicador(QWidget *parent) : QWidget(parent), ui(new Ui::Publicador)  {
    ui->setupUi(this);

    QIcon icon("../recursos/icon.jpg");
    this->setWindowIcon(icon);

//    qDebug() << "La base se conecto" << AdminDB::getInstance()->conectar("../db/baseFotoYa.sqlite");
//    this->hashtag = "merylucas2015";

#define TuFotoYa
//#define OsimaniApp
//#define QF
//#define Maina

#ifdef TuFotoYa
    client_id = "169885556709356";
    client_secret = "48256c90767089251d99b41488ab1d0e";
    redirect_uri = "https://www.facebook.com/connect/login_success.html";
    user_id = "me";  // Id de Juan Gabriel
#endif
#ifdef OsimaniApp
    client_id = "389285021141638";
    client_secret = "01baa740fb520bab7e4483d5acfd9cce";
    redirect_uri = "https://www.facebook.com/connect/login_success.html";
#endif
#ifdef QF
    client_id = "1433969450248723";
    client_secret = "7d0401cd2371ccfa2372b1a8502c810e";
    redirect_uri = "https://www.facebook.com/connect/login_success.html";
#endif
#ifdef Maina
    client_id = "184448581913869";
    client_secret = "80d15b296b51e40f76315b5e65e22ea1";
    redirect_uri = "https://www.facebook.com/connect/login_success.html";
    page_id = "953590381396148"; // FotoYa
    user_id = "me";  // Id de Juan Gabriel
#endif


    connect(ui->wv, SIGNAL(loadFinished(bool)), this, SLOT(slot_loginFacebookCargado(bool)));

    manager = new QNetworkAccessManager(this);

    QUrl url("https://www.facebook.com/dialog/oauth");
        // client_id / 169885556709356
        // redirect_uri / https://www.facebook.com/connect/login_success.html
        // scope / public_profile,user_friends,user_likes,user_location,user_photos,user_posts

    QUrlQuery urlQuery(url);  // Para generar los parametros de la url.

    QList<QPair<QString, QString> > listaParametros;
    listaParametros.append(QPair<QString, QString>("client_id", client_id));
    listaParametros.append(QPair<QString, QString>("redirect_uri", redirect_uri));
    listaParametros.append(QPair<QString, QString>("response_type", "code"));
    listaParametros.append(QPair<QString, QString>("scope", "public_profile,user_friends,user_likes,user_location,user_photos,user_posts,publish_actions"));
    urlQuery.setQueryItems(listaParametros);

    url.setQuery(urlQuery);

    qDebug() << "URL prinicpla de facebook" << url;

    // Para ver el proceso de login ver: https://developers.facebook.com/docs/facebook-login/manually-build-a-login-flow
    ui->wv->load(url);

    connect(ui->pbIngresar, SIGNAL(clicked()), this, SLOT(slot_autenticar()));
    connect(ui->leClave, SIGNAL(returnPressed()), this, SLOT(slot_autenticar()));

    connect(ui->pbPublicarAlbum, SIGNAL(clicked()), this, SLOT(slot_crearAlbum()));

    connect( ui->pbVolver, SIGNAL( pressed() ), this, SLOT( slot_volver() ) );
}

Publicador::~Publicador()  {
    delete ui;
}


void Publicador::cargarHastHagsEnComboBox()
{
    ui->cbHashTags->clear();

    QStringList hashtagList;
    hashtagList += "Elejir un HashTag";
    hashtagList += Comun::getInstance()->getHashTags();
    ui->cbHashTags->addItems( hashtagList );

}

void Publicador::slot_loginFacebookCargado(bool exito)  {
    if (exito)  {
        QWebFrame * frame = ui->wv->page()->mainFrame();
        QWebElement document = frame->documentElement();

        QWebElement usuario = document.findFirst("input[id=\"email\"]");
        QWebElement clave = document.findFirst("input[id=\"pass\"]");

        if ( ! usuario.isNull() && ! clave.isNull() )  {  // Si se cumple es porque encontro el login
            // Desconectamos porque es solo para obtener el login de Facebook
            disconnect( ui->wv, NULL, NULL, NULL );

            ui->pbIngresar->setEnabled(true);
            ui->leUsuario->setEnabled(true);
            ui->leClave->setEnabled(true);
            ui->leClave->setFocus();
        }
    }
    else  {
        QMessageBox::critical(this, "No se pudo contactar con Facebook", "Es probable sea el accesso a internet.");
        return;
    }
}

void Publicador::slot_autenticar()  {

    QWebFrame * frame = ui->wv->page()->mainFrame();
    QWebElement document = frame->documentElement();

    QWebElement usuario = document.findFirst("input[id=\"email\"]");
    QWebElement clave = document.findFirst("input[id=\"pass\"]");

    if ( usuario.isNull() && clave.isNull() )  {  // No se pudieron obtener los tags de usuario y clave
//        ui->labelInfo->setText("<h2 style=\"color:#ff0055\">Intente de nuevo");
        return;
    }

//    ui->labelInfo->setText("Accediendo a Twitter ...");
    ui->pbIngresar->setEnabled(false);
    ui->leUsuario->setEnabled(false);
    ui->leClave->setEnabled(false);

    usuario.setAttribute("value", ui->leUsuario->text());
    clave.setAttribute("value", ui->leClave->text());

    connect( ui->wv, SIGNAL( loadFinished( bool ) ), this, SLOT( slot_autenticacionCargada( bool ) ) );

//    QWebElement permitir = document.findFirst("input[type=\"submit\"]");
    QWebElement permitir = document.findFirst("button[id=\"loginbutton\"]");
    permitir.evaluateJavaScript("this.click()");
}

/**
 * @brief Publicador::slot_autenticacionCargada Se invoca cuando el usuario se loguea en facebook (la primera
 * vez pedira autorizacion para que TuFotoYa pueda publicar en tu muro).
 * Este metodo se invoca cuando nos logueamos, en este momento recibe un code, y con este arma una solicitud
 * a https://graph.facebook.com/v2.5/oauth/access_token con varios parametros (incluido el code) que sera
 * enviado con QNetorkAccessManager
 * @param exito
 */
void Publicador::slot_autenticacionCargada(bool exito)  {
    if (exito)  {
        // Cuando enviamos el usuario y clave solo viene en GET la key 'code'
        QString code = QUrlQuery(ui->wv->url()).queryItemValue("code");

        qDebug() << "code recibido" << code;

        QUrl url("https://graph.facebook.com/v2.5/oauth/access_token");
        QUrlQuery urlQuery(url);  // Para generar los parametros de la url.

        QList<QPair<QString, QString> > listaParametros;
        listaParametros.append(QPair<QString, QString>("client_id", client_id));
        listaParametros.append(QPair<QString, QString>("redirect_uri", redirect_uri));
        listaParametros.append(QPair<QString, QString>("client_secret", client_secret));
        listaParametros.append(QPair<QString, QString>("code", code));
        urlQuery.setQueryItems(listaParametros);

        url.setQuery(urlQuery);

        // Desconectamos porque era solo para obtener el code
        disconnect( ui->wv, NULL, NULL, NULL );

        connect( manager, SIGNAL( finished( QNetworkReply * ) ),
                 this, SLOT( slot_envioCodeFinalizada( QNetworkReply * ) ) );
        manager->get(QNetworkRequest(url));
    }
    else  {
        QMessageBox::critical(this, "No se obtuvo respuesta de la autenticacion", "Es probable sea el accesso a internet.");
        return;
    }
}

void Publicador::slot_envioCodeFinalizada(QNetworkReply *reply)  {

    QByteArray ba = reply->readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(ba);

    if ( jDoc.isObject() )  {
        QJsonObject jObj = jDoc.object();

        access_token = jObj.value("access_token").toString();
        // Vienen otros dos parametros que no se usaran:
        // token_type
        // expires_in

        qDebug() << "access token recibido" << access_token;

        // Porque solo era para obtener el access token
        disconnect( manager, NULL, NULL, NULL );

        // Aqui ya podemos decir que estamos listos para publicar, entonces habilitamos el QGroupBox del album
        if ( ! access_token.isEmpty() )  {
            ui->gbAlbum->setEnabled(true);
            ui->wv->setEnabled(false);
        }
    }
}

/**
 * @brief Publicador::slot_crearAlbum Cuando ya tenemos el access_token, se habilita el groupbox para elegir
 * el hashtag y se habilita el boton Publicar. Este boton invoca a este metodo.
 * El presionar el boton es porque ya se eligio el hashtag, entocnes se bloquea el combobox
 */
void Publicador::slot_crearAlbum()  {

    // Para que quede fijo el hashtag y que toda la publicacion se realice con este hashtag
    ui->cbHashTags->setEnabled( true );


    QUrl url("https://graph.facebook.com/v2.5/" + user_id + "/albums");
    QUrlQuery urlQuery(url);  // Para generar los parametros de la url.

    QList<QPair<QString, QString> > listaParametros;
    listaParametros.append(QPair<QString, QString>("name", ui->leNombreAlbum->text()));
    listaParametros.append(QPair<QString, QString>("message", ui->leDescrAlbum->text()));
    listaParametros.append(QPair<QString, QString>("access_token", access_token));
    urlQuery.setQueryItems(listaParametros);

    url.setQuery(urlQuery);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    connect( manager, SIGNAL( finished( QNetworkReply * ) ),
             this, SLOT( slot_creacionAlbumFinalizada( QNetworkReply * ) ) );

    qDebug() << "POST para crear album"  << url;

    manager->post(req, QByteArray());

}

void Publicador::slot_creacionAlbumFinalizada(QNetworkReply *reply)  {
    QByteArray ba = reply->readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(ba);

    qDebug() << "Respuesta sobre la creacion del album slot_creacionAlbumFinalizada" << jDoc;

    if ( jDoc.isObject() )  {
        QJsonObject jObj = jDoc.object();

        id_album = jObj.value("id").toString();

        bool exito = AdminDB::getInstance()->addIdAlbum( ui->cbHashTags->currentText(), id_album );

        if ( exito )  {
            // Se hizo UPDATE correcto. Es decir, el hashtag existe y no se habia creado el album aun
            disconnect( manager, NULL, NULL, NULL );

            connect( manager, SIGNAL( finished( QNetworkReply * ) ),
                     this, SLOT( slot_publicacionFotoFinalizada( QNetworkReply * ) ) );

            this->publicarProximaFoto();
        }
        else  {  // inconsistencia. Posiblemente el album ya existe
            QMessageBox::warning(this, "Problemas!!", "Es posible que el album ya haya sido creado.");
        }
    }
}


// Lo pude hacer andar gracias a: https://github.com/eamocanu/Facebook.Qt.APIs
void Publicador::publicarProximaFoto()  {

    QString filename = "../fotos/" + ui->cbHashTags->currentText()
            + "_imagenes/" + AdminDB::getInstance()->getFotoParaUpload( ui->cbHashTags->currentText() );

    qDebug() << filename;

    QFile foto( filename, this );
    QFileInfo fileInfo( foto );

    if ( ! foto.open(QIODevice::ReadOnly) ) {
        qDebug() << "No se pudo abrir el archivo para lectura";
        return;
    }

    QUrl url( "https://graph.facebook.com/v2.5/" + id_album + "/photos" );

    QString boundary = "---------------------------17673466415141";

    QByteArray data;

    data.append("--"+QString(boundary)+"\r\n");
    data.append("Content-Disposition: form-data; name=\"access_token\"\r\n\r\n");
    data.append(access_token +"\r\n");

    data.append("--"+QString(boundary)+"\r\n");
    data.append("Content-Disposition: form-data; name=\"message\"\r\n\r\n");
    data.append("#" + ui->cbHashTags->currentText() );
    data.append("\r\n");

    data.append("--"+QString(boundary)+"\r\n");
    data.append("Content-Disposition: form-data; name=\"name\"; filename=\"" + fileInfo.fileName() + "\"\r\n");
    data.append("Content-Type: image/jpg\r\n\r\n");

    data.append(foto.readAll());
    data.append("\r\n");

    data.append("--"+QString(boundary)+"--\r\n");
    foto.close();

    QNetworkRequest request(url);

    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Language", "en-us,en;q=0.5");
    request.setRawHeader("Accept-Encoding", "gzip,deflate");
    request.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
    request.setRawHeader("Keep-Alive", "300");
    request.setRawHeader("Connection", "keep-alive");

    request.setRawHeader("Content-Type", QString("multipart/form-data; boundary=" + QString(boundary)).toLatin1());
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

    qDebug() << "SE envia el POST"  << url << "\nDATA size" << data.size();

    manager->post( request, data );
}

void Publicador::slot_publicacionFotoFinalizada(QNetworkReply *reply)  {
    QByteArray ba = reply->readAll();
    qDebug() << ba;

    this->publicarProximaFoto();
}

void Publicador::slot_volver()
{
    disconnect( ui->wv, NULL, NULL, NULL );
    disconnect( manager, NULL, NULL, NULL );

    emit signal_volver();
}


void Publicador::setTabFocus( bool focus )
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
