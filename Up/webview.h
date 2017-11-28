#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWidget>
#include <QWebView>

class WebView : public QWebView
{
    Q_OBJECT

public:
    WebView(QWidget *parent = 0 );

protected:
    void closeEvent( QCloseEvent * );

signals:
    void signal_webViewCerradoDesdeLaCruz();
};

#endif // WEBVIEW_H
