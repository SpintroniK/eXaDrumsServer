/*
 * CppSockets.hpp
 *
 *  Created on: 28 Dec 2017
 *      Author: jeremy
 */

#ifndef SOURCE_CPPSOCKETS_HPP_
#define SOURCE_CPPSOCKETS_HPP_

#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>


namespace Sockets
{

	static constexpr int MAXHOSTNAME = 200;
	static constexpr int MAXCONNECTIONS = 5;

	class Socket
	{

	public:

		Socket() : socketId(-1), address({0})
		{

		}

		virtual ~Socket()
		{
			if(IsValid())
			{
				::close(socketId);
				//std::cout << "Close " << socketId << std::endl;
			}
		}

		bool Create()
		{
			socketId = socket(AF_INET, SOCK_STREAM, 0);

			//std::cout << "Create: " << socketId << std::endl;

			int on = 1;
			if(setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) == -1)
				return false;

			return IsValid();
		}

		bool Bind(int port)
		{
			if(!IsValid())
			{
				return false;
			}

			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons(port);

			int bind_return = ::bind(socketId, ( struct sockaddr * ) &address, sizeof(address));

			if(bind_return == -1)
			{
				return false;
			}

			return true;
		}

		bool Listen() const
		{
			if(!IsValid())
		    {
				return false;
		    }

			int listen_return = ::listen(socketId, MAXCONNECTIONS);

			if(listen_return == -1)
			{
				return false;
			}

			return true;
		}

		bool Accept(Socket& new_socket) const
		{

			int addr_length = sizeof(address);
			new_socket.socketId = ::accept(socketId, (sockaddr*)&address, reinterpret_cast<socklen_t*>(&addr_length));

			//std::cout << "Accept: " << socketId << " => " << new_socket.socketId << std::endl;

			if(new_socket.socketId <= 0)
			{
				return false;
			}
			else
		    {
				return true;
		    }
		}

		int Receive(std::string& s, std::size_t length = 2048) const
		{

			std::string buffer(length, '\0');

			int status = ::recv(socketId, (void*)buffer.data(), buffer.size(), 0);

			//std::cout << status << std::endl;

			switch(status)
			{
				case -1:
					//std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
					return 0;
				case 0: return 0;
				default:
				{
					std::string temp(buffer.data());
					s.swap(temp);
					return status;
				}
			}


		}

		bool Send(const std::string& s) const
		{
			int status = ::send(socketId, s.data(), s.size(), MSG_NOSIGNAL);

			return status != -1;
		}

		bool IsValid() const { return socketId != -1; }

	private:

		int socketId;
		sockaddr_in address;

	};

	class ServerSocket : private Socket
	{

	public:

		ServerSocket(int port)
		{
			Socket::Create();
			Socket::Bind(port);
			Socket::Listen();
			//Socket::Accept(*this);
		}

		ServerSocket()
		{

		}

		virtual ~ServerSocket()
		{

		}

		void Accept(ServerSocket& sock)
		{
			Socket::Accept(sock);
		}

		const ServerSocket& operator>>(std::string& s) const
		{
			Socket::Receive(s);
			return *this;
		}

		const ServerSocket& operator<<(const std::string& s) const
		{
			Socket::Send(s);
			return *this;
		}

	private:


	};


}  // namespace Sockets


#endif /* SOURCE_CPPSOCKETS_HPP_ */

