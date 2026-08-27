/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 12:14:40 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/08/26 16:54:07 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "http/HTTPRequest.hpp"
#include "http/HTTPResponse.hpp"
#include "server/ServerSocket.hpp"

class Client
{
private:
	//Info del poropio cliente.
	int _fd;
	ServerSocket &_serverSocket; //La referencia al serversocker al que pertenece el cliente. Esto es necesario para poder acceder a la configuracion del server desde el cliente, por ejemplo para obtener el root o el error_page.
	std::string _recvBuffer;
	bool _keepAlive;
	//Para una petición concreta.
	std::string _sendBuffer;
	size_t _bytesSent;
	HTTPRequest _request;
	HTTPResponse _response;

public:
	Client(int clientFd, ServerSocket &serverSocket);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();

	// Getters
	int getFd() const;
	ServerSocket &getServerSocket() const;

	std::string &getRecvBuffer();
	const std::string &getRecvBuffer() const;

	std::string &getSendBuffer();
	const std::string &getSendBuffer() const;

	size_t getBytesSent() const;

	HTTPRequest &getRequest();
	const HTTPRequest &getRequest() const;

	HTTPResponse &getResponse();
	const HTTPResponse &getResponse() const;

	bool isKeepAlive() const;

	// Setters / acciones
	void setKeepAlive(bool keepAlive);

	void addBytesSent(size_t bytes);
	void resetBytesSent();

	void clearRecvBuffer();
	void clearSendBuffer();

	void reset();
};

#endif