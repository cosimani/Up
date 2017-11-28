#include "image.hpp"

Image::Image( QWidget *parent ) :
    QWidget( parent )
{
    mgr = new QNetworkAccessManager();
    mgra = new QNetworkAccessManager();
    connect( mgr, SIGNAL( finished(QNetworkReply*)), SLOT(listo(QNetworkReply*)));
    connect( mgra, SIGNAL( finished(QNetworkReply*)), SLOT(listoa(QNetworkReply*)));
}

void Image::setImage( QString image, int fillPolicy )
{
    this->image.load( ":/images/" + image );
    if( !this->image.isNull() )
    {
        this->repaint();
    }
    this->policy = fillPolicy;
}

void Image::setImageUrl( QString url, int fillPolicy )
{
    this->policy = fillPolicy;

    mgra->get( QNetworkRequest( QUrl( url ) ) );

}

void Image::soyLogo()
{
    this->policy = ADJUST;
    QString query( "http://www.vayra.com.ar/periosalud/periosalud.php?key=p3riosalud&action=logoUrl" );

    mgr->get( QNetworkRequest( QUrl( query ) ) );
}

void Image::paintEvent( QPaintEvent* )
{
    QPainter painter( this );

    if( ! image.isNull() )
    {
        bool aspectDifference = image.width() / ( float )image.height() < this->width() / ( float )this->height();

        int newWidth = aspectDifference == policy ? this->width() : image.width() * this->height() / ( float )image.height();
        int newHeight = aspectDifference == policy ? image.height() * this->width() / ( float )image.width() : this->height();

        QImage resized = image.scaled( newWidth, newHeight );

        if( ! resized.isNull() )
        {
            painter.drawImage( this->width() / ( float )2 - resized.width() / ( float )2,
                               this->height() / ( float )2 - resized.height() / ( float )2,
                               resized );
        }
    }
}

void Image::resizeEvent( QResizeEvent* )
{
    this->repaint();
}

#include <QDebug>
void Image::listo(QNetworkReply *r)
{
    QString replyString( QString::fromLatin1( r->readAll() ) );

    if( replyString != "" )
    {
        qDebug() << "Hay link mira" << replyString;
        mgra->get(QNetworkRequest(QUrl(replyString)));
    }
}
void Image::listoa(QNetworkReply *r)
{
    this->image.loadFromData( r->readAll() );
    if( !this->image.isNull() )
    {
        qDebug() << "Hay todo";
        this->repaint();
    }
    this->policy = ADJUST;
}

