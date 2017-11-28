#ifndef IMAGE_HPP
#define IMAGE_HPP

#define FILL   true
#define ADJUST false

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Image : public QWidget
{
    Q_OBJECT

private:
    QImage image;
    bool policy;

    QNetworkAccessManager *mgr;
    QNetworkAccessManager *mgra;
    QString logourl;

public:
    explicit Image( QWidget *parent = NULL );
    void setImage( QString image, int policy );
    void setImageUrl( QString url, int policy = ADJUST );
    void soyLogo();

protected:
    void paintEvent( QPaintEvent* );
    void resizeEvent( QResizeEvent* );

private slots:
    void listo( QNetworkReply*r);
    void listoa( QNetworkReply*r);
};

#endif // IMAGE_HPP

