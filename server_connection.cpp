#include "server_connection.h"
#include <string>
#include <iostream>

ServerConnection::ServerConnection(std::vector<WorkplaceSlot>* p_workplaces) :
	m_port(27015)
{
	config(m_port, 32);
	m_workplaces = p_workplaces;
}

ServerConnection::ServerConnection(const int& p_port, int p_max_clients, std::vector<WorkplaceSlot>* p_workplaces) :
	m_port(p_port),
	m_workplaces(p_workplaces)
{
	config(m_port, p_max_clients);
}

ServerConnection::~ServerConnection()
{
	delete[] m_clients; //usuwanie zarezerwowanej pamięci na klientów
	shutdown(m_server, SD_BOTH); //wyłączanie serwera
}

void ServerConnection::config(const int& p_port, int p_max_clients)
{
	//uruchamianie winsock
	std::clog<<"Starting up winsock... ";
	
	if (WSAStartup(MAKEWORD(2, 1), &m_wsa_data) != 0)
	{
		std::cerr<<"Error while starting winsock! (WSAStartup())"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("WSAStartup() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	std::clog<<"done!"<<std::endl;
	
	m_IP_address = get_local_IP(); //pobieranie informacji o adresie IP w sieci lokalnej
	
	std::clog<<"Local IPV4 address: "<<m_IP_address<<std::endl;
	std::clog<<"Setting up server... ";
	
	m_server = socket(AF_INET, SOCK_STREAM, 0);
	m_clients = new SOCKET[p_max_clients];
	u_long blocking = 1;
	
	if (ioctlsocket(m_server, FIONBIO, &blocking) != 0)
	{
		std::cerr<<"Error - ioctlsocket()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("ioctlsocket() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	//konfiguracja adresu IP oraz portu
	m_address.sin_addr.s_addr = inet_addr(m_IP_address);
	m_address.sin_port = htons(m_port);
	m_address.sin_family = AF_INET;
	
	//konfiguracja zmiennych
	FD_ZERO(&master);
	FD_ZERO(&tmp_master);
	
	std::clog<<"done!"<<std::endl;
	std::clog<<"Starting up server... ";
	
	//rozpoczynanie nasłuchiwania
	if (bind(m_server, (SOCKADDR*)&m_address, sizeof(m_address)) == SOCKET_ERROR)
	{
		std::cerr<<"Error - bind()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("bind() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	if (listen(m_server, 32) == SOCKET_ERROR)
	{
		std::cerr<<"Error - listen()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("listen() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	FD_SET(m_server, &master);
	
	std::clog<<"done!"<<std::endl;
}

void ServerConnection::run()
{
	int num_of_connections;
	int client_id;
	int bytes_read;
	char data[256];
	
	while(true)
	{
		tmp_master = master;
		
		if ((num_of_connections = select(NULL, &tmp_master, NULL, NULL, NULL)) == SOCKET_ERROR)
		{
			std::cerr<<"Error - select()"<<std::endl;
			std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
			throw std::runtime_error(strcat("select() -> ", std::to_string(WSAGetLastError()).c_str()));
		}
		
		for (int i=0; num_of_connections>i; i++)
		{
			//jeżeli przyszło coś na gniazdo serwera to znaczy że nowy klient
			client_id = tmp_master.fd_array[i];
			if ((SOCKET)client_id == m_server)
			{
				client_id = accept(m_server, NULL, NULL); //akceptuj połączenie
				
				if (client_id == SOCKET_ERROR)
				{
					std::cerr<<"Error - accept()"<<std::endl;
					std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
					throw std::runtime_error(strcat("accept() -> ", std::to_string(WSAGetLastError()).c_str()));
				}
				
				FD_SET((SOCKET)client_id, &master);
				
				std::clog<<"Nowy klient! ID: "<<client_id<<std::endl;
			}
			else
			{
				#ifdef QT_DEBUG
				
				std::clog<<"Klient ID: "<<client_id<<std::endl;
				
				#endif
				memset(data, 0, sizeof(data));
				bytes_read = recv(client_id, data, sizeof(data), NULL);
				
				if (bytes_read == -1)
				{
					std::cerr<<"Error - recv()"<<std::endl;
					std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
					
					if (WSAGetLastError() != 0)
						throw std::runtime_error(strcat("recv() -> ", std::to_string(WSAGetLastError()).c_str()));
				}
				
				if (bytes_read == 0)
				{
					#ifdef QT_DEBUG

					std::clog<<"Polaczenie zakonczone!"<<std::endl;
					
					#endif
					FD_CLR((SOCKET)client_id, &master);
				}
				else
				{
					#ifdef QT_DEBUG
					
					std::clog<<"Dane odebrane: '"<<data<<"'"<<std::endl;
					
					#endif
					
					if (strcmp(data, "get status") == 0)
					{
						send_workplaces_status(client_id);
					}
					
					if (strstr(data, "login") != NULL)
					{
						register_workplace(client_id, data);
					}
				}
			}
		}
	}
}

char* ServerConnection::get_local_IP()
{	
	char hostname[100];
	
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		std::cerr<<"Error - gethostname()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("gethostname() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	LPHOSTENT host_info = gethostbyname(hostname);
	
	if (host_info == 0)
	{
		std::cerr<<"Error - gethostbyname()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("gethostbyname() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	struct in_addr ipv4_addr_struct;
	
	if (host_info->h_addr_list[1] != 0)
		memcpy(&ipv4_addr_struct, host_info->h_addr_list[1], sizeof(struct in_addr));
	else
		memcpy(&ipv4_addr_struct, host_info->h_addr_list[0], sizeof(struct in_addr));
	
	const char* host = inet_ntoa(ipv4_addr_struct);
	host_info = gethostbyname(host);
	
	if (host_info == 0)
	{
		std::cerr<<"Error - gethostbyname()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("gethostbyname() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	unsigned int ipv4_addr_uint = inet_addr(host);
	host_info = gethostbyaddr((char*)&ipv4_addr_uint, 4, AF_INET);
	
	if (host_info == 0)
	{
		std::cerr<<"Error - gethostbyaddr()"<<std::endl;
		std::cerr<<"Error number: "<<WSAGetLastError()<<std::endl;
		throw std::runtime_error(strcat("gethostbyaddr() -> ", std::to_string(WSAGetLastError()).c_str()));
	}
	
	return inet_ntoa(ipv4_addr_struct);
}

void ServerConnection::send_workplaces_status(int &p_client)
{
	std::string data = "";
	
	for (WorkplaceSlot& slot : *m_workplaces)
	{
		data += std::to_string(slot.get_workplace_number());
		data += ";";
		data += std::to_string(slot.get_occupied_by());
		data += ";";
	}
	
	if (send(p_client, data.c_str(), data.length(), 0) > 0)
	{
		#ifdef QT_DEBUG
		
		std::clog<<"Przeslano informacje o statusie stanowisk: '"<<data<<"'"<<std::endl;
		
		#endif
	}
	else
	{
		std::cerr<<"Error while sending workplaces status to CLIENT: "<<p_client<<std::endl;
	}
}

void ServerConnection::register_workplace(int &p_client, char* data)
{
	data = strtok(data, ";"); //usunięcie z wiadomości napisu 'login;'
	data = strtok(NULL, ";");
	
	int selected_workplace = atoi(data);
	
	data = strtok(NULL, ";");
	
	int coded_workplace = atoi(data);
	
	std::cout<<"ID klienta: "<<p_client<<std::endl;
	std::cout<<"Wybrane stanowisko: "<<selected_workplace<<std::endl;
	std::cout<<"Zakodowane stanowisko: "<<coded_workplace<<std::endl;
	
	
	const auto success = m_connection_workplaces.insert({p_client, selected_workplace}).second;
	
	if (success)
	{
		send(p_client, "OK", 2, 0);
	}
	else
	{
		send(p_client, "ERR;1;", 6, 0);
	}
}
