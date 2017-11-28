#ifndef BOTON_H
#define BOTON_H

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QTimer>

namespace Ui {
class Boton;
}

class Boton : public QWidget
{
    Q_OBJECT

public:
    explicit Boton(QWidget *parent = 0);
    ~Boton();

    void setColor( QString color );
    void setTexto( QString texto );


private:
    Ui::Boton *ui;

    QImage iFondo;
    QColor colorOriginal, color;

    bool isPressed;

protected:
    void paintEvent( QPaintEvent * );
    void mousePressEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void mouseMoveEvent(QMouseEvent * e);

signals:
    void clic();

private slots:
    void slot_demoraParaEmitirClic();

};

#endif // BOTON_H
