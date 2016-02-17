#include <QApplication>
#include "js_widget.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

int main(int argc, char *argv[]){
	try{
        //application
		QApplication app(argc, argv);

        //widget
        yourNamespace::JsWidget js_widget;
        js_widget.resize(500,500);
        js_widget.show();

        const int num_changes = 10;
        for(int t = 0; t < num_changes; ++t){
            const int num_points = 100;
            std::vector<std::pair<double,double>> data;
            for(int i = 0; i < num_points; ++i){
                data.push_back(std::pair<double,double>((std::rand()%1000)/1000.,(std::rand()%1000)/1000.));
            }
            js_widget.onSetData(data);
            QApplication::processEvents();
            std::this_thread::sleep_for (std::chrono::seconds(1));
        }

        return app.exec();
    }
	catch(std::logic_error& ex){ std::cout << "Logic error: " << ex.what();}
	catch(std::runtime_error& ex){ std::cout << "Runtime error: " << ex.what();}
	catch(...){ std::cout << "An unknown error occurred";}
}
