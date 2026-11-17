/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 12:42:34 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/16 19:19:27 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "server/Client.hpp"
#include "colors.hpp"

/**
 * Constructor for the Client class.
 * 
 * @param clientFd The file descriptor for the client socket.
 * @param serverSocket The server socket to which the client is connected.
 */
Client::Client(int clientFd, ServerSocket &serverSocket)
	: _fd(clientFd), _serverSocket(serverSocket), _keepAlive(false), _lastActivity(time(NULL)),
		_bytesSent(0), _fileFd(-1), _fileBytesRemaining(0)
{
	std::cout << BOLD_GREEN << "Client constructor called " << this << RESET << std::endl;
}

/**
 * Copy constructor for the Client class.
 * 
 * @param other The Client object to copy from.
 * @warning: ALERT: This copy constructor copies the file descriptor, which can lead to issues if both Client objects try to use or close the same socket. Use with caution.
 * @warning: It's used to create a new Client object while we add new clients to de clienst map in the Server class. Be careful when using this constructor in other contexts, It can lead to double closing of the same socket. Use the constructor with caution and ensure that the socket is managed properly to avoid resource leaks or undefined behavior.
 */
Client::Client(const Client &other)
	: _fd(other._fd), _serverSocket(other._serverSocket), _recvBuffer(other._recvBuffer), _keepAlive(other._keepAlive),
	  _lastActivity(other._lastActivity), _sendBuffer(other._sendBuffer), _bytesSent(other._bytesSent),
	   _fileFd(other._fileFd), _fileBytesRemaining(other._fileBytesRemaining), _request(other._request)
{
	std::cout << BOLD_GREEN << "Client copy constructor called " << this << " from " << &other << RESET << std::endl;
}

// /**
//  * Assignment operator for the Client class.
//  * 
//  * @param other The Client object to assign from.
//  * @return A reference to the assigned Client object.
//  */
// Client &Client::operator=(const Client &other)
// {
// 	if (this != &other)
// 	{
// 		_fd = other._fd;
// 		_serverSocket = other._serverSocket;
// 		_recvBuffer = other._recvBuffer;
// 		_lastActivity = other._lastActivity;
// 		_sendBuffer = other._sendBuffer;
// 		_bytesSent = other._bytesSent;
// 		_keepAlive = other._keepAlive;
// 		_request = other._request;
// 	}
// 	std::cout << BOLD_GREEN << "Client assignment operator called" << this << " from " << &other << RESET << std::endl;
// 	return *this;
// }

/**
 * Destructor for the Client class.
 */
Client::~Client()
{
	std::cout << BOLD_RED << "Client destructor called "  << this << RESET << std::endl;
}

/**
 * Get the file descriptor for the client socket.
 * 
 * @return The file descriptor for the client socket.
 */
int Client::getFd() const
{
	return _fd;
}

/**
 * Get the server socket for the client.
 * 
 * @return A reference to the server socket.
 */
ServerSocket &Client::getServerSocket() const
{
	return _serverSocket;
}

/**
 * Get the receive buffer for the client.
 * 
 * @return A reference to the receive buffer string.
 */
std::string &Client::getRecvBuffer()
{
	return _recvBuffer;
}

/**
 * Get the receive buffer for the client (const version).
 * 
 * @return A const reference to the receive buffer string.
 */
const std::string &Client::getRecvBuffer() const
{
	return _recvBuffer;
}

/**
 * Get the send buffer for the client.
 * 
 * @return A reference to the send buffer string.
 */
std::string &Client::getSendBuffer()
{
	return _sendBuffer;
}

/**
 * Get the send buffer for the client (const version).
 * 
 * @return A const reference to the send buffer string.
 */
const std::string &Client::getSendBuffer() const
{
	return _sendBuffer;
}

/**
 * Get the number of bytes sent to the client.
 * 
 * @return The number of bytes sent to the client.
 */
size_t Client::getBytesSent() const
{
	return _bytesSent;
}

/**
 * Get the file descriptor for the file being sent to the client.
 * 
 * @return The file descriptor for the file being sent to the client.
 */
int Client::getFileFd() const
{
	return _fileFd;
}

/**
 * Get the number of bytes remaining to be sent for the file being sent to the client.
 * 
 * @return The number of bytes remaining to be sent for the file being sent to the client.
 */
size_t Client::getFileBytesRemaining() const
{
	return _fileBytesRemaining;
}

/**
 * Check if the client connection should be kept alive.
 * 
 * @return True if the connection should be kept alive, false otherwise.
 */
bool Client::isKeepAlive() const
{
	return _keepAlive;
}

/**
 * Get the last activity time for the client.
 * 
 * @return The last activity time.
 */
time_t Client::getLastActivity() const
{
	return _lastActivity;
}

/**
 * Get the HTTPRequest object for the client.
 * 
 * @return A reference to the HTTPRequest object.
 */
HTTPRequest &Client::getRequest()
{
	return _request;
}

/**
 * Get the HTTPRequest object for the client (const version).
 * 
 * @return A const reference to the HTTPRequest object.
 */
const HTTPRequest &Client::getRequest() const
{
	return _request;
}

/**
 * Set whether the client connection should be kept alive.
 * 
 * @param keepAlive True to keep the connection alive, false otherwise.
 */
void Client::setKeepAlive(bool keepAlive)
{
	_keepAlive = keepAlive;
}

/**
 * Set the last activity time for the client.
 * 
 * @param lastActivity The last activity time.
 */
void Client::setLastActivity(time_t lastActivity)
{
	_lastActivity = lastActivity;
}

/**
 * Add bytes to the count of bytes sent to the client.
 * 
 * @param bytes The number of bytes to add.
 */
void Client::addBytesSent(size_t bytes)
{
	_bytesSent += bytes;
}

/**
 * Reset the count of bytes sent to the client.
 */
void Client::resetBytesSent()
{
	_bytesSent = 0;
}

/**
 * Subtract bytes from the count of bytes remaining to be sent for the file being sent to the client.
 * 
 * @param bytes The number of bytes to subtract.
 */
void Client::subtractFileBytesRemaining(size_t bytes)
{
	if (_fileBytesRemaining >= bytes)
		_fileBytesRemaining -= bytes;
	else
		_fileBytesRemaining = 0;
}
/**
 * Clear the receive buffer for the client.
 */
void Client::clearRecvBuffer()
{
	_recvBuffer.clear();
}

/**
 * Clear the send buffer for the client.
 */
void Client::clearSendBuffer()
{
	_sendBuffer.clear();
}

/**
 * Clear the HTTPRequest object for the client.
 */
void Client::clearRequest()
{
	_request = HTTPRequest();
}

/**
 * Reset the client state, Only the attributes referring to the current request/response.
 * It restarts the state of a client when a request has been fully processed and the connection is still alive, so that it can be ready to process a new request.
 * It does not reset the file descriptor, server socket reference, or keep-alive status, as those are associated with the client connection itself and should persist across multiple requests.
 */
void Client::reset()
{
	closeFile();
	clearSendBuffer();
	resetBytesSent();
	clearRequest();
}

/**
 * Check if the client is currently sending a file.
 * 
 * @return True if the client is sending a file, false otherwise.
 */
bool Client::isSendingFile() const
{
	return _fileFd != -1;
}

/**
 * Set the file descriptor and size for the file being sent to the client.
 * 
 * @param fileFd The file descriptor of the file.
 * @param fileSize The size of the file in bytes.
 */
void Client::setFile(int fileFd, size_t fileSize)
{
	_fileFd = fileFd;
	_fileBytesRemaining = fileSize;
}

/**
 * Close the file being sent to the client.
 */
void Client::closeFile()
{
	if (_fileFd != -1)
	{
		close(_fileFd);
		_fileFd = -1;
	}
	_fileBytesRemaining = 0;
}