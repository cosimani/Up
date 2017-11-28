#ifndef GALERIA_H
#define GALERIA_H

#include <QWidget>
#include <QtGlobal>

namespace Ui {
class Galeria;
}

class Galeria : public QWidget
{
    Q_OBJECT

public:
    explicit Galeria(QWidget *parent = 0);
    ~Galeria();

private:
    Ui::Galeria *ui;
    QImage imageOriginal;
    QString archivoImagen;
    QString archivoImagenOriginal;

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void slot_fotoEnGaleria(QString archivoImagen);
    void slot_fotoOriginalEnGaleria(QString archivoImagenOriginal);

private slots:
    void slot_imprimir();
    void slot_imprimir10x15();
    void slot_imprimir15x21();

};

#endif // GALERIA_H
