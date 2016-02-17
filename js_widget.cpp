#include "js_widget.h"

#include <QFileDialog>
#include <QLayout>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QFile>
#include <QDir>
#include <sstream>
#include <iostream>


//call to Q_INIT_RESOURCE should be outside of any namespace
void loadJsWidgetResources(){
    Q_INIT_RESOURCE(js_widget);
}

namespace yourNamespace {

    JsWidget::JsWidget(QWidget *parent):
        _webView(nullptr),
        _mainFrame(nullptr),
        _connection_ready(false)
    {
        loadJsWidgetResources();
        initUI();
    }

    QString JsWidget::readTextFromFile(QString filename){
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            std::stringstream ss;
            ss << "Unable to open the file: " << filename.toStdString();
            throw std::logic_error(ss.str());
        }
        QTextStream in(&file);
        return in.readAll();
    }

    void JsWidget::initUI(){
        _webView = new QWebView(this);

        connect(_webView, &QWebView::loadFinished, this, &JsWidget::onWebViewFinishedLoading);

        _mainFrame = _webView->page()->mainFrame();
        _mainFrame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
        _mainFrame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

        QObject::connect(_mainFrame,&QWebFrame::javaScriptWindowObjectCleared,this,&JsWidget::onConnectJs);
        if(!_webView->settings()->testAttribute(QWebSettings::JavascriptEnabled)){
            throw std::runtime_error("Javascript is not enabled!");
        }

        QString html = readTextFromFile(":/myWidget/widget.html");
        _webView->setHtml(html, QUrl("qrc:/myWidget/"));
    }

    void JsWidget::onJsLog(QString text){
        QTextStream(stdout)  << "JS Log:\t" << text << endl;
    }
    void JsWidget::onJsError(QString text){
        throw std::runtime_error(text.toStdString().c_str());
    }

    void JsWidget::onConnectJs(){
        _mainFrame->addToJavaScriptWindowObject("Qt", this);
    }

    void JsWidget::onWebViewFinishedLoading(bool ok){
        _connection_ready = true;
        _webView->page()->setViewportSize(size());
    }

    JsWidget::~JsWidget(){
    }

    void JsWidget::onSetData(std::vector<std::pair<double,double>>& points){
        std::stringstream ss;
        ss << "x,y" << std::endl;
        for(auto& p: points){
            ss << std::get<0>(p) << "," << std::get<1>(p) << std::endl;
        }
        while(!_connection_ready){//it is better to use condition_variable
           QApplication::processEvents();
       }

        emit sgnSetData(QString::fromStdString(ss.str()));
    }

    void JsWidget::resizeEvent(QResizeEvent * e){
        _webView->page()->setViewportSize(size());
    }
}

