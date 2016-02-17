#ifndef JS_WIDGET_H
#define JS_WIDGET_H

#include <QWidget>
#include <QApplication>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <thread>

class QLayout;
class QWebView;
class QWebFrame;
class QResizeEvent;

namespace yourNamespace {

    class JsWidget: public QWidget
    {
        Q_OBJECT
    public:
        explicit JsWidget(QWidget *parent = 0);
        ~JsWidget();

    //Slots used by C++
    public slots:
        void onSetData(std::vector<std::pair<double,double>>& points);

    //Slots used by JS
    public slots:
        void onJsLog(QString text);
        void onJsError(QString text);

    private:
        void initUI();
        QString readTextFromFile(QString filename);
        void asyncSetData(std::vector<std::pair<double,double>> points);

    //JS connection handling
    private slots:
        void onWebViewFinishedLoading(bool ok);
        void onConnectJs();

    signals:
        void sgnSetData(QString);

    protected:
        virtual void resizeEvent(QResizeEvent * e);

    private:
        QWebView* _webView;
        QWebFrame* _mainFrame;
        bool _connection_ready;
        std::mutex _mutex;
        std::condition_variable _cv;
        std::thread _connection_thread;
    };

}


#endif // JS_WIDGET_H
