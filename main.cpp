#include "mainwindow.h"
#include <QApplication>
#include "client_connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	ClientConnection connection("192.168.0.178", 27015, &w);
	
    w.show();
    return a.exec();
}
