#include "webview.h"

WebView::WebView(QWidget * parent) : QWebView(parent)
{

}

void WebView::closeEvent(QCloseEvent *)
{
    emit signal_webViewCerradoDesdeLaCruz();
}
