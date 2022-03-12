#include "workerslot.h"
#include <iostream>

WorkerSlot::WorkerSlot(int p_x, int p_y, QWidget* p_parent_window) :
	m_x(p_x),
	m_y(p_y),
	m_status(SlotStatus::NOT_CONNECTED),
	m_label(new QLabel(p_parent_window))
{
	if (!m_img.load("data\\slot_not_connected.png"))
	{
		std::cerr<<"Error while loading image 'data\\slot_not_connected.png'"<<std::endl;
	}
	else
	{
		m_label->move(m_x, m_y);
		m_label->setPixmap(m_img);
	}
}

void WorkerSlot::set_x(int p_x)
{
	m_x = p_x;
	m_label->move(m_x, m_y);
}

void WorkerSlot::set_y(int p_y)
{
	m_y = p_y;
	m_label->move(m_x, m_y);
}

void WorkerSlot::set_x_y(int p_x, int p_y)
{
	m_x = p_x;
	m_y = p_y;
	m_label->move(m_x, m_y);
}

void WorkerSlot::set_status(SlotStatus p_status)
{
	m_status = p_status;
}
