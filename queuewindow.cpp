#include "queuewindow.h"
#include "ui_queuewindow.h"
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include <string>

QueueWindow::QueueWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::QueueWindow)
{
	ui->setupUi(this);
	
	//pobieranie wymiarów ekranu
	QRect screen = QApplication::desktop()->availableGeometry();
	
	m_screen_width = screen.width();
	m_screen_height = screen.height();
	
	ui->label_2->move(10, 10);
	
	//ui->label_2->setGeometry(screen_width, screen_height, ui->label_2->width(), ui->label_2->height());
	
	QLabel* jakis_label = new QLabel(this);
	jakis_label->move(m_screen_width*0.9, m_screen_height*0.9);
	QPixmap img("icon.ico");
	jakis_label->setPixmap(img);
	
	//jakis_label->setObjectName(QStringLiteral("label_5"));
	//jakis_label->setGeometry(QRect(, 100, 100));
	//jakis_label->setText(QApplication::translate("QueueWindow", "SIEMA", Q_NULLPTR));
	
	load_worker_slots();
}

QueueWindow::~QueueWindow()
{
	delete ui;
}


void QueueWindow::load_worker_slots()
{
	m_worker_slots.push_back(WorkerSlot(100, 100, this));
	m_worker_slots.push_back(WorkerSlot(100, 200, this));
	
	/*std::fstream file("data\\properties.conf", std::ios::in);
	std::string line, str_file;
	int no_of_slots;
	
	if (file.good())
	{
		file>>no_of_slots;
		
		for (int i=0; no_of_slots>i; i++)
		{
			//TODO: wygenerować sloty
			m_worker_slots.push_back(WorkerSlot(100*no_of_slots, 100, this));
		}
	}*/
}
