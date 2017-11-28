#ifndef PUBLICADOR_H
#define PUBLICADOR_H

#include <QWidget>
#include <QByteArray>
#include <QUrlQuery>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QImage>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>

namespace Ui {
class Publicador;
}

class Publicador : public QWidget
{
    Q_OBJECT

public:
    explicit Publicador(QWidget *parent = 0);
    ~Publicador();

    void cargarHastHagsEnComboBox();

    void setTabFocus(bool focus);
private:
    Ui::Publicador *ui;
    QNetworkAccessManager * manager;
    QString access_token, client_id, client_secret, redirect_uri, page_id, user_id, id_album;
    void publicarProximaFoto();

private slots:
    void slot_loginFacebookCargado(bool);
    void slot_autenticar();
    void slot_autenticacionCargada(bool exito);
    void slot_envioCodeFinalizada(QNetworkReply *reply);
    void slot_crearAlbum();
    void slot_creacionAlbumFinalizada(QNetworkReply *reply);
    void slot_publicacionFotoFinalizada(QNetworkReply *reply);

    void slot_volver();

signals:
    void signal_volver();
};

#endif // PUBLICADOR_H
