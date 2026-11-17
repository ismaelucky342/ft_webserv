/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 12:14:40 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/16 19:18:41 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "http/HTTPRequest.hpp"
#include "server/ServerSocket.hpp"
#include <unistd.h> // close()

class Client
{
private:
	//Info del poropio cliente.
	int _fd;
	ServerSocket &_serverSocket; //La referencia al serversocker al que pertenece el cliente. Esto es necesario para poder acceder a la configuracion del server desde el cliente, por ejemplo para obtener el root o el error_page.
	std::string _recvBuffer;
	bool _keepAlive;
	time_t _lastActivity;
	//Para una petición concreta.
	std::string _sendBuffer;
	size_t _bytesSent;
	int _fileFd; // Descriptor del archivo que se está enviando al cliente, si es que se está enviando un archivo.
	size_t _fileBytesRemaining; // Cantidad de bytes restantes por enviar del archivo que se está enviando al cliente, si es que se está enviando un archivo.
	HTTPRequest _request;

	Client &operator=(const Client &other);

public:
	Client(int clientFd, ServerSocket &serverSocket);
	Client(const Client &other);
	~Client();

	// Getters
	int getFd() const;
	ServerSocket &getServerSocket() const;

	std::string &getRecvBuffer();
	const std::string &getRecvBuffer() const;

	std::string &getSendBuffer();
	const std::string &getSendBuffer() const;

	size_t getBytesSent() const;

	int getFileFd() const;
	size_t getFileBytesRemaining() const;

	bool isKeepAlive() const;

	time_t getLastActivity() const;

	HTTPRequest &getRequest();
	const HTTPRequest &getRequest() const;

	// Setters / acciones
	void setKeepAlive(bool keepAlive);
	void setLastActivity(time_t lastActivity);

	void addBytesSent(size_t bytes);
	void resetBytesSent();

	void subtractFileBytesRemaining(size_t bytes);

	void clearRecvBuffer();
	void clearSendBuffer();
	void clearRequest();

	void reset();

	// Gestión del envío de archivos.
	bool isSendingFile() const;
	void setFile(int fileFd, size_t fileSize);
	void closeFile();
};

#endif