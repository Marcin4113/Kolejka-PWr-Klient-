#include "client_connection.h"
#include <iostream>

ClientConnection::ClientConnection(const char *p_IP_address, const int &p_port, MainWindow* p_window) :
	m_IP_address(p_IP_address),
	m_port(p_port),
	m_window(p_window)
{
	int status;
	
	std::clog<<"Starting up winsock... ";
	
	//uruchomienie winsocka
	if (WSAStartup(MAKEWORD(2, 1), &m_wsa_data) != 0)
	{
		std::cerr<<"Error while starting winsock! (WSAStartup())"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("WSAStartup() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	std::clog<<"done!"<<std::endl;
	std::clog<<"Setting up client... ";
	
	status = socket(AF_INET, SOCK_STREAM, 0);
	
	if (status == SOCKET_ERROR)
	{
		std::cerr<<"Error - socket()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("socket() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	m_connection = status;
	
	//ustawienie adresu do komunikacji z serwerem
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = inet_addr(m_IP_address);
	m_address.sin_port = htons(m_port);
	std::clog<<"done!"<<std::endl;
	
	std::cout<<"Server IP: "<<m_IP_address<<std::endl;
	std::cout<<"Server port: "<<m_port<<std::endl;
	std::clog<<"Connecting to the server... ";
	
	if (connect(m_connection, (SOCKADDR*)&m_address, sizeof(m_address)) == SOCKET_ERROR)
	{
		std::cerr<<"Error - connect()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("connect() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	std::clog<<"connected!"<<std::endl;
	
	char buf[256];
	memset(buf, 0, sizeof(buf));
	
	send(m_connection, "get status", strlen("get status"), 0);
	
	recv(m_connection, buf, sizeof(buf), 0);
	
	std::cout<<"buf: "<<buf<<std::endl;
	
	char* tmp;
	char tmp2[100];
	tmp = strtok(buf, ";");
	
	for (int i=1; tmp != NULL; i++)
	{
		memset(tmp2, 0, sizeof(100));
		strcat(tmp2, "Stanowisko nr. ");
		strcat(tmp2, std::to_string(i).c_str());
		
		m_window->add_elem(tmp2);
		if (strcmp(tmp, "1") == 0)
			m_window->disable_elem(i-1);
		
		tmp = strtok(NULL, ";");
	}
}

ClientConnection::~ClientConnection()
{
	shutdown(m_connection, SD_BOTH);
}
