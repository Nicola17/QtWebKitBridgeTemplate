project(QtWebKitBridgeTemplate)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebKit REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)

qt5_add_resources(Resources js_widget.qrc)

add_executable (test js_widget.cpp test.cpp ${Resources})

target_link_libraries (test Qt5::Widgets)
target_link_libraries (test Qt5::OpenGL)
target_link_libraries (test Qt5::WebKit)
target_link_libraries (test Qt5::WebKitWidgets)
