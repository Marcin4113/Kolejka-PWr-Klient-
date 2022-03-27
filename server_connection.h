#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H
#include <winsock2.h>
#include <QtCore>
#include "workplaceslot.h"

class ServerConnection : public QThread
{
    public:
		ServerConnection(std::vector<WorkplaceSlot>* p_workplaces); //lokalny adres IP oraz domyślny port
		ServerConnection(const int& p_port, int p_max_clients, std::vector<WorkplaceSlot>* p_workplaces); //lokalny adres IP z wybranym portem
		~ServerConnection();
		
		void config(const int& p_port, int p_max_clients);
		void run();
		
	private:
		const char* m_IP_address; //adres IP gdzie ma być postawiony serwer
		const int m_port; //port na którym ma być postawiony serwer
		
		std::vector<WorkplaceSlot>* m_workplaces; //stanowiska pracowników
		std::map<int, int> m_connection_workplaces; //zmapowane stanowiska i wskaźniki na obiekty stanowisk
		
		WSADATA m_wsa_data; //dane wersji winsocka
		SOCKET m_server; //wskaźnik na serwer
		SOCKET* m_clients; //klienci podłączeni do serwera
		SOCKADDR_IN m_address; //struktura przechowująca dane połączenia
		
		fd_set master;
		fd_set tmp_master;
		
		char* get_local_IP(); //funkcja zwracająca lokalne IP komputera
		void send_workplaces_status(int& p_client); //funkcja wysyła do klienta informację o statusie stanowisk
		void register_workplace(int& p_client, char* data); //zarejestruj nowo zalogowane stanowisko
};

#endif // SERVER_CONNECTION_H
