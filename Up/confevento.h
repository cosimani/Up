#ifndef CONFEVENTO_H
#define CONFEVENTO_H

#include <QDialog>
#include <QDir>
#include <QFileInfoList>
#include <QStringList>
#include <QFileInfo>
#include <QList>
#include "comun.h"

namespace Ui {
class ConfEvento;
}

class ConfEvento : public QDialog
{
    Q_OBJECT

public:
    explicit ConfEvento(QWidget *parent = 0);
    ~ConfEvento();

    void setTabFocus(bool focus);

    void configurarWidgetPara( Comun::TipoEvento tipoEvento );

    void cargarPlantillaDeDisco();

    void cargarHastHagsEnComboBox();


private:
    Ui::ConfEvento *ui;


private slots:
    void slot_aceptar();
    void slot_cancelar();
    void slot_cambioComboBoxDetectado();
    void slot_cambioLineEditDetectado();

    /**
     * @brief slot_cambioComboBoxPapel Se invoca cuando se cambia la seleccion del papel
     */
    void slot_cambioComboBoxPapel();

    /**
     * @brief slot_cambioComboBoxImpresion Se invoca cuando se cambia la seleccion de la impresion o PDF
     */
    void slot_cambioComboBoxImpresion();


signals:
    /**
     * @brief signal_evento_nuevo Se abre Search en modo Crear evento
     */
    void signal_evento_nuevo();

    /**
     * @brief signal_evento_existente Se abre Search en modo Evento existente
     */
    void signal_evento_existente();

    /**
     * @brief signal_rechazo Para cuando se pulsa Cancelar, volvera a Principal.
     */
    void signal_cancelar();

    /**
     * @brief signal_confEvento_listoParaIniciarSearch Esta todo configurado para que Search inicie. Ya que todos
     * los parametros estan seteados, EXISTENTE, ONLINE, OFFLINE, etc etc.
     */
    void signal_confEvento_listoParaIniciarSearch();

};

#endif // CONFEVENTO_H
