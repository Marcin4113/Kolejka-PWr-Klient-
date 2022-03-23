#ifndef QUEUEWINDOW_H
#define QUEUEWINDOW_H

#include <QDialog>
#include <QLabel>
#include "workplaceslot.h"

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
		float m_screen_scale; //skala ekranu względem oryginału
		float m_slots_scale; //skala obrazków stanowisk
		std::vector<WorkplaceSlot> m_workplaces; //wektor stanowisk Pań z dziekanatu
};

#endif // QUEUEWINDOW_H
