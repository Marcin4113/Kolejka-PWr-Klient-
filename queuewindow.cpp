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
	//pozwala na (w miarę dobre) odwzorowanie wielkości elementów na różnych ekranach
	m_screen_scale = (float)(m_screen_width * m_screen_height) / (ORIGINAL_SCREEN_WIDTH * ORIGINAL_SCREEN_HEIGHT);
	
	//ładowanie obrazka zawierającego mapę stanowisk
	QLabel* map_label = new QLabel(this);
	map_label->setGeometry(0, 0, m_screen_width, m_screen_height);
	
	QPixmap map_pixmap("..\\data\\map.png");
	map_pixmap = map_pixmap.scaled(m_screen_width, m_screen_height, Qt::KeepAspectRatio);
	map_label->setPixmap(map_pixmap);
	
	#ifdef QT_DEBUG
	
	std::clog<<std::endl<<"map_label width: "<<map_label->width()<<std::endl;
	std::clog<<"map_label height: "<<map_label->height()<<std::endl<<std::endl;
	
	/*TYLKO TESTY
	QLabel* jakis_label = new QLabel(this);
	jakis_label->move(m_screen_width*0.9, m_screen_height*0.9);
	QPixmap img("..\\icon.ico");
	jakis_label->setPixmap(img);
	
	//jakis_label->setObjectName(QStringLiteral("label_5"));
	//jakis_label->setGeometry(QRect(, 100, 100));
	//jakis_label->setText(QApplication::translate("QueueWindow", "SIEMA", Q_NULLPTR));*/
	
	#endif
	
	load_worker_slots();
}

QueueWindow::~QueueWindow()
{
	delete ui;
}

void QueueWindow::load_worker_slots()
{
	std::fstream file("..\\data\\properties.conf", std::ios::in);
	
	if (file.good())
	{
		int slot_num, pos_x, pos_y; //numer slotu, pozycja
		float scale; //skala obiektów (stanowisk)
		std::string line; //line przechowuje aktualną linię z pliku
		
		while (!file.eof())
		{
			std::getline(file, line);
			
			//jeżeli linia nie zaczyna się od komentarza i nie jest pusta
			if (line[0] != '#' && line != "")
			{
				//jeżeli dana linia zawiera informację o skali obrazków
				if (line.find("scale:") != std::string::npos)
				{
					line.erase(0, 7); //usunięcie słówka 'scale:'
					scale = std::atof(line.c_str());
					
					#ifdef QT_DEBUG
					
					std::clog<<std::endl<<"file: objects scale: "<<scale<<std::endl<<std::endl;
					
					#endif
				}
				
				//jeżeli dana linia zawiera informację o danym stanowisku
				if (line.find("slot:") != std::string::npos)
				{
					//usunięcie słówka 'slot: '
					line.erase(0, 6);
					slot_num = std::atoi(line.c_str());
					
					//usunięcie spacji żeby poznać pozycję x
					line.erase(0, line.find(" ")+1);
					pos_x = std::atoi(line.c_str());
					
					//usunięcie spacji żeby poznać pozycję y
					line.erase(0, line.find(" ")+1);
					pos_y = std::atoi(line.c_str());
					
					try 
					{
						m_workplaces.push_back(WorkplaceSlot(pos_x * m_screen_width / 100.0, pos_y * m_screen_height / 100.0, scale, slot_num, this));
						
						#ifdef QT_DEBUG
						
						std::clog<<"Created WorplaceSlot (num: "<<slot_num<<", pos_x: "<<pos_x * m_screen_width / 100.0<<
								   ", pos_y: "<<pos_y * m_screen_height / 100.0<<", scale: "<<scale<<")"<<std::endl;
						
						#endif						
					}
					catch (std::invalid_argument e) 
					{
						std::cerr<<"Can't create object WorplaceSlot (num: "<<slot_num<<", pos_x: "<<pos_x * m_screen_width / 100.0<<
								   ", pos_y: "<<pos_y * m_screen_height / 100.0<<", scale: "<<scale<<")\nwhat(): "<<e.what()<<std::endl;
					}
				}
			}
		}
	}
	else
	{
		std::cerr<<"Can't open file 'properties.conf'! Error: "<<strerror(errno)<<std::endl;
	}
}
