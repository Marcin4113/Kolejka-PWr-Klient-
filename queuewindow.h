#ifndef QUEUEWINDOW_H
#define QUEUEWINDOW_H

#include <QDialog>
#include <QLabel>
#include "workerslot.h"

namespace Ui {
	class QueueWindow;
}

class QueueWindow : public QDialog
{
		Q_OBJECT
		
	public:
		explicit QueueWindow(QWidget *parent = nullptr);
		~QueueWindow();
		
		void load_worker_slots();
	private:
		Ui::QueueWindow *ui;
		
		int m_screen_width; //wysokość ekranu
		int m_screen_height; //szerokość ekranu
		std::vector<WorkerSlot> m_worker_slots; //wektor stanowisk Pań z dziekanatu
		//QLabel** m_worker_labels;
};

#endif // QUEUEWINDOW_H
