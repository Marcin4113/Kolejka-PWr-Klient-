#include "mainwindow.h"
#include "queuewindow.h"
#include <QApplication>
#include "client_connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	//ClientConnection connection("192.168.1.158", 27015, &w);
	
    w.show();
	
	QueueWindow qw;
	qw.showFullScreen();
    return a.exec();
}
