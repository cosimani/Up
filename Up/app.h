#ifndef APP_H
#define APP_H

#define APP_NAME "UP - Tu foto ya"

#include <QObject>
#include <QImage>

#include "navegador.h"
#include "search.h"
#include "principal.h"
#include "confevento.h"
#include "publicador.h"

class App : public Navegador
{
    Q_OBJECT

private:
    Principal *principal;
    ConfEvento *confEvento;
    Search *search;
    Publicador* publicador;

    QImage imFondo;


protected:
    void paintEvent( QPaintEvent * );

    void keyPressEvent(QKeyEvent *e);

    void closeEvent(QCloseEvent *e);

public:
    explicit App( QWidget *parent = NULL );
    virtual ~App();

    Principal *getPrincipal() const;
    void setPrincipal(Principal *value);

private slots:


    /**
     * @brief slot_crearEvento Para cuando se pulsa el boton Crear Evento en la ventana Principal.
     * Este slot es para abrir ConfEvento con la configuracion de Evento nuevo..
     */
    void slot_crearEvento();

    /**
     * @brief slot_eventoExistente Para cuando se pulsa el boton Evento existente en la ventana Principal.
     * Este slot es para abrir ConfEvento con la configuracion de Evento existente.
     */
    void slot_eventoExistente();

    /**
     * @brief slot_eventoNuevo Para cuando se pulsa Aceptar para crear un evento en ConfEvento.
     * Este slot es para abrir Search.
     */
    void slot_eventoNuevo();

    /**
     * @brief slot_abrirPrincipal Este slot se usa para abrir Principal, para empezar todo de nuevo.
     * Como si volvieramos a abrir la aplicacion.
     */
    void slot_abrirPrincipal();

    void slot_evento_existente();


    void slot_proyectar();

    void slot_publicar();


    void slot_listoParaIniciarSearch();
};

#endif // APP_H
