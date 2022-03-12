#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H
#include <winsock2.h>
#include "mainwindow.h"

class ClientConnection
{
	public:
		ClientConnection(const char* p_IP_address, const int& p_port, MainWindow* p_window);
		~ClientConnection();
		
	private:
		WSADATA m_wsa_data;
		SOCKET m_connection;
		SOCKADDR_IN m_address;
		
		const char* m_IP_address; //adres IP serwera
		const int m_port; //port do komunikacji z serwerem
		MainWindow* m_window; //potrzebne w celu modyfikacji elementów w oknie
};

#endif // CLIENT_CONNECTION_H
