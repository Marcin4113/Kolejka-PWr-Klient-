#ifndef WORKERSLOT_H
#define WORKERSLOT_H
#include <QPixmap>
#include <QLabel>

enum class SlotStatus
{
	NOT_CONNECTED,
	FREE,
	BUSY,
	BREAK
};

class WorkerSlot
{
	public:
		WorkerSlot(int p_x, int p_y, QWidget* p_parent_window);
		
		//gettery
		int get_x();
		int get_y();
		SlotStatus get_status();
		
		//settery
		void set_x(int p_x);
		void set_y(int p_y);
		void set_x_y(int p_x, int p_y);
		void set_status(SlotStatus p_status);
		
	private:
		int m_x;
		int m_y;
		SlotStatus m_status;
		QPixmap m_img;
		QLabel* m_label;
};

#endif // WORKERSLOT_H
