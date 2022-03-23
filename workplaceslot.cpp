#include "workplaceslot.h"
#include <iostream>

WorkplaceSlot::WorkplaceSlot(int p_x, int p_y, float p_scale, int p_workplace_number, QWidget* p_parent_window)
{
	m_x = p_x;
	m_y = p_y;
	m_scale = p_scale;
	m_workplace_num = p_workplace_number;
	m_status = SlotStatus::NOT_CONNECTED;
	
	//ładowanie obrazka stanowiska
	if (!load_img("..\\data\\slot_not_connected.png", p_parent_window))
	{
		throw std::invalid_argument("Cannot load image 'data\\slot_not_connected.png'");
	}
}

void WorkplaceSlot::set_x(int p_x)
{
	m_x = p_x;
	m_label->move(m_x, m_y);
}

void WorkplaceSlot::set_y(int p_y)
{
	m_y = p_y;
	m_label->move(m_x, m_y);
}

void WorkplaceSlot::set_x_y(int p_x, int p_y)
{
	m_x = p_x;
	m_y = p_y;
	m_label->move(m_x, m_y);
}

void WorkplaceSlot::set_status(SlotStatus p_status)
{
	m_status = p_status;
	
	//ładowanie obrazka stanowiska
	switch(m_status)
	{
		case SlotStatus::NOT_CONNECTED:
			if (!load_img("..\\data\\slot_not_connected.png"))
			{
				throw std::invalid_argument("Cannot load image 'data\\slot_not_connected.png'");
			}
			break;
		
		case SlotStatus::FREE:
			if (!load_img("..\\data\\slot_free.png"))
			{
				throw std::invalid_argument("Cannot load image 'data\\slot_free.png'");
			}
			break;
		
		case SlotStatus::BUSY:
			if (!load_img("..\\data\\slot_busy.png"))
			{
				throw std::invalid_argument("Cannot load image 'data\\slot_busy.png'");
			}
			break;
		
		case SlotStatus::BREAK:
			if (!load_img("..\\data\\slot_break.png"))
			{
				throw std::invalid_argument("Cannot load image 'data\\slot_break.png'");
			}
			break;
	}
}

bool WorkplaceSlot::load_img(const char* path, QWidget* p_parent_window)
{
	if (!m_img.load(path))
	{
		std::cerr<<"Error while loading image 'data\\slot_not_connected.png'"<<std::endl;
		return false;
	}
	
	//stworzenie uchwytu na obiekt jeżeli przekazano parametr na którym oknie ma on być (obiekt)
	if (p_parent_window != nullptr)
		m_label = new QLabel(p_parent_window);
	
	m_label->move(m_x, m_y); //ustawianie uchwytu w odpowiedne miejsce na ekranie
	m_img = m_img.scaled(m_img.width() * m_scale, m_img.height() * m_scale, Qt::KeepAspectRatio); //zmiana rozmiaru obrazka stanowiska
	m_label->setPixmap(m_img); //ładowanie obrazka do uchwytu
	
	return true;
}
