#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QImage>
#include <QIcon>
#include <QShowEvent>
#include <QColor>
#include <QVector>
#include <QFile>

#include <QSize>

#include <QByteArray>
#include <QUrlQuery>
#include <QMessageBox>
#include <QWebPage>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>
#include <QRect>
#include <QSize>
#include <QTimer>
#include <QDir>
#include "proyectador.h"
#include <QDesktopWidget>


namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = 0);
    ~Search();

    void setTabFocus(bool focus);

    /**
     * @brief iniciar Comienza la busqueda de fotos y comienza el proceso de impresion (en PDF o en impresora)
     */
    void iniciar();

private:
    Ui::Search *ui;

    QNetworkAccessManager * managerFoto;

    QStringList getImagenesInstagram();

    bool isFile(QString archivo);

    int cuantasImagenesHastaAhoraInstagram;
    QTimer * timerScrollInstagram, * timerUpdateInstagram;

    int cantidadDeCiclosIrFinalIrArriba;


private slots:
    void slot_descargaFotoFinalizada(QNetworkReply *reply);

    void slot_mostrarProyectador();

    void slot_wvActualizadoInstagram(bool exito);
    void slot_contentChangeInstagram(QSize);

    void slot_irFinalInstagram();
    void slot_updatePaginaInstagram();

    void slot_mostrarWebInstagram();

    void slot_actualizarTimerDesdeLineEdits();

    /**
     * @brief slot_salirDelEvento Salir del evento para volver a Principal
     */
    void slot_salirDelEvento();

    /**
     * @brief slot_cerrarWebView Solo esta para que destildar el checkbox que muestra el WebView cuando
     * el usuario lo cierra desde la cruz
     */
    void slot_cerrarWebView();

    /**
     * @brief slot_recibeNotificacionTimersActualizados En este slot volvemos a leer desde Comun el valor de
     * todos los timers para setearlos de nuevo dentro de los QLineEdits y en sus respectivos setIntervals
     */
    void slot_recibeNotificacionTimersActualizados();

protected:
    void keyPressEvent(QKeyEvent *e);

    void showEvent( QShowEvent * );

signals:
    void signal_salirDelEvento();
};

#endif // SEARCH_H
