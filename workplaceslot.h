#ifndef WORKPLACESLOT_H
#define WORKPLACESLOT_H
#include <QPixmap>
#include <QLabel>

#define ORIGINAL_SCREEN_WIDTH 1920
#define ORIGINAL_SCREEN_HEIGHT 1200

enum class SlotStatus
{
	NOT_CONNECTED,
	FREE,
	BUSY,
	BREAK
};

class WorkplaceSlot
{
	public:
		//pozycja x i y stanowiska, skala stanowiska (skala obrazka), wskaĹşnik do okna w ktĂłrym tworzone sÄ… stanowiska
		WorkplaceSlot(int p_x, int p_y, float p_scale, int p_workplace_number, QWidget* p_parent_window);
		
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
		int m_x; //współrzędna x stanowiska
		int m_y; //współrzędna y stanowiska
		float m_scale; //scala stanowiska
		unsigned short m_workplace_num; //numer stanowiska
		//(obrazek ma domyślnie wymiary 50x50, aby ładnie wyglądało na ekranie docelowym można zmieniać skalę)
		SlotStatus m_status; //status stanowiska (wolne, zajęte, itd)
		QPixmap m_img; //obrazek stanowiska (odpowiedni kolor)
		QLabel* m_label; //uchwyt trzymający obrazek wraz z pozycją i wymiarem
		
		bool load_img(const char* path, QWidget* p_parent_window = nullptr); //ładowanie obrazka z pliku do m_img
};

#endif // WORKPLACESLOT_H
