#ifndef IMPRESOR_H
#define IMPRESOR_H

#include <QWidget>
#include <QTimer>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QTreeWidgetItem>
#include <QSqlTableModel>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>
#include <QPrinter>
#include <QPainter>
#include <QFileInfo>
#include <QRectF>
#include <QBrush>
#include <QColor>

namespace Ui {
class Impresor;
}

class Impresor : public QWidget
{
    Q_OBJECT

public:
    explicit Impresor(QWidget *parent = 0);
    ~Impresor();

    /**
     * @brief iniciar Inicia los timers para que actualice la tableWidget e imprima en PDF o en la impresora, o
     * bien, segun el seteo puede actualizar la tableWidget solamente, sin imprimir ni PDF ni en impresora
     */
    void iniciar();

    void setTimerImpresiones( int milisegundos );
    void setTimerDir( int milisegundos );

    /**
     * @brief cancelar Cancela la actualizacion de la tableWidget y detiene los timers para que no imprima nada
     * ni PDF ni en impresora
     */
    void cancelar();

private:
    Ui::Impresor *ui;
    QTimer *timerDir, *timerImpresion;
    QStringList listaFotos;  // Lista de fotos en el directorio

    QSqlRelationalTableModel* tableModel;

    int selectedIndex;

private slots:
    void slot_timerDir();
    void slot_timerImpresion();
    void slot_updateTabla();
    void slot_itemChanged(const QModelIndex & current, const QModelIndex &);

public slots:
    void slot_setIndexFoto(int index);



signals:
    void signal_fotoEnGaleria(QString);
    void signal_fotoOriginalEnGaleria(QString);


};

#endif // IMPRESOR_H
