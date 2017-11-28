#include "boton.h"
#include "ui_boton.h"

#include <QPainter>
#include <QPalette>
#include <QMouseEvent>

Boton::Boton(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Boton),
                                isPressed( false )
{
    ui->setupUi(this);

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
    this->setPalette( palette );

//    QFont font("Helvetica", this->width()/20, QFont::Bold);
//    QFont font("BethHand", this->width()/20, QFont::Bold);
//    QFont font("Avant que...", this->width()/20, QFont::Bold);
    QFont font("Angelina", this->width()/15, QFont::Bold);
    ui->lTexto->setFont(font);

//    this->setMouseTracking( true );
}

Boton::~Boton()
{
    delete ui;
}

// e91e63 pink / 9c27b0 purple / 673ab7 deep purple / 3f51b5 indigo / 009688 teal /
// http://desarrollador-android.com/material-design/diseno-material-design/estilo/color/
void Boton::setColor(QString color)
{
    this->colorOriginal = QColor( color );
    this->color = this->colorOriginal;
    this->repaint();
}

void Boton::setTexto(QString texto)
{
    ui->lTexto->setText( texto );
}

void Boton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    color.setAlpha( 190 );

    painter.fillRect( 0,0, this->width(), this->height(), color );
}

void Boton::mousePressEvent(QMouseEvent *)
{
    this->color.setRgba( qRgba( colorOriginal.red() + 20, colorOriginal.green() + 20,
                                colorOriginal.blue() + 20, colorOriginal.alpha() - 150 ) );
    this->isPressed = true;

    QTimer::singleShot( 750, this, SLOT( slot_demoraParaEmitirClic() ) );

    this->repaint();
}

void Boton::mouseReleaseEvent(QMouseEvent *)
{
    this->color.setRgba( qRgba( colorOriginal.red(), colorOriginal.green(),
                                colorOriginal.blue(), colorOriginal.alpha() ) );
    this->isPressed = false;
    this->repaint();
}

void Boton::mouseMoveEvent(QMouseEvent *e )
{
    if ( e->pos().x() <= 0 || e->pos().x() >= this->width() ||
         e->pos().y() <= 0 || e->pos().y() >= this->height() )  {

        this->color.setRgba( qRgba( colorOriginal.red(), colorOriginal.green(),
                                    colorOriginal.blue(), colorOriginal.alpha() ) );
        this->repaint();
    }
}

void Boton::slot_demoraParaEmitirClic()
{
    emit clic();
}
