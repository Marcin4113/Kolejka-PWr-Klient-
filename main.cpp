#include "mainwindow.h"
#include "queuewindow.h"
#include <QApplication>
#include "server_connection.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow main_window; //główne okno
	QueueWindow queue_window; //okno z mapą stanowisk
	ServerConnection connection(&queue_window.get_workplaces()); //połączenie ze stanowiskami
	
    //main_window.show(); //status każdego stanowiska
	//queue_window.showFullScreen();
	connection.start();
	
	int status = a.exec();
	
	connection.terminate();
	
    return status;
}
