#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>

#include <QWebView>

namespace Ui {
class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = 0);
    ~Principal();

    void setTabFocus(bool focus);
protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::Principal *ui;

private slots:
    void slot_crearEvento();
    void slot_eventoExistente();
    void slot_proyectar();
    void slot_publicar();
    void slot_salir();

signals:
    /**
     * @brief signal_crearEvento Para cuando se pulsa el boton Crear evento
     */
    void signal_crearEvento();

    /**
     * @brief signal_eventoExistente Para cuando se pulsa Evento existente
     */
    void signal_eventoExistente();

    void signal_proyectar();
    void signal_publicar();
    void signal_salir();
};

#endif // PRINCIPAL_H
