/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:34:35 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/31 16:09:54 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // close()
#include <sstream>	// std::ostringstream
#include <cstdlib>	// exit()
#include <netdb.h>	// getaddrinfo(), freeaddrinfo()

#include "server/ServerSocket.hpp"
#include <cstring>

/**
 * Constructor for the ServerSocket class.
 *
 * @param listen The listen configuration object.
 */
ServerSocket::ServerSocket(const Listen &listen)
	: _listen(&listen), _defaultConfig(NULL), _addrInfo(NULL), _serverSocketFd(-1)
{
	std::cout << BOLD_GREEN << "ServerSocket constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the ServerSocket class.
 *
 * @param other The ServerSocket object to copy from.
 */
ServerSocket::ServerSocket(const ServerSocket &other)
	: _listen(other._listen), _configs(other._configs), _defaultConfig(other._defaultConfig), _addrInfo(other._addrInfo), _serverSocketFd(other._serverSocketFd)
{
	std::cout << BOLD_GREEN << "ServerSocket copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the ServerSocket class.
 *
 * @param other The ServerSocket object to assign from.
 * @return A reference to the assigned ServerSocket object.
 */
ServerSocket &ServerSocket::operator=(const ServerSocket &other)
{
	if (this != &other)
	{
		_listen = other._listen;
		_configs = other._configs;
		_defaultConfig = other._defaultConfig;
		_addrInfo = other._addrInfo;
		_serverSocketFd = other._serverSocketFd;
	}
	std::cout << BOLD_GREEN << "ServerSocket assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the ServerSocket class. It closes the server socket to free up resources.
 */
ServerSocket::~ServerSocket()
{
	if (_serverSocketFd != -1)
		close(_serverSocketFd);
	std::cout << BOLD_RED << "ServerSocket destructor called" << RESET << std::endl;
}

/**
 * Sets up the address information for the server socket.
 */
void ServerSocket::setupAddressInfo()
{
	addrinfo
		hints; // Estructura que contiene información sobre el tipo de socket que queremos crear. La usamos para indicarle al sistema operativo qué tipo de socket queremos crear y cómo queremos que se comporte.
	std::ostringstream portStream;

	portStream << _listen->getPort();
	std::string port = portStream.str();

	memset(&hints, 0, sizeof(hints));

	hints.ai_family =
		AF_INET; // AF_INET for IPv4, AF_INET6 for IPv6, AF_UNSPEC for any address family
	hints.ai_socktype = SOCK_STREAM; // TCP socket
	hints.ai_flags = AI_PASSIVE;	 // Socket will be used for binding

	if (getaddrinfo(_listen->getInterface().c_str(), port.c_str(), &hints, &_addrInfo) !=
		0) // (Parametros: host, port, hints(La receta de cómo queremos crear el socket), result (la estructura donde se guardará la info de la dirección. Mirar en http.md para entender la estructura addrinfo))
	{
		std::cerr << "Error getting address info" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Frees the address information for the server socket.
 */
void ServerSocket::freeAddressInfo()
{
	if (_addrInfo)
	{
		freeaddrinfo(_addrInfo);
		_addrInfo = NULL;
	}
}

/**
 * Creates the server socket.
 */
void ServerSocket::createSocket()
{
	_serverSocketFd = socket(_addrInfo->ai_family, _addrInfo->ai_socktype, _addrInfo->ai_protocol);
	std::cout
		<< "Socket descriptor: " << _serverSocketFd
		<< std::
			   endl; //IMP: Se podria borrar. solo era para debugueary entender bien el flujo de la creacion del socket.
	if (_serverSocketFd == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Binds the server socket to the specified port and address.
 */
void ServerSocket::bindSocket()
{
	if (bind(_serverSocketFd, _addrInfo->ai_addr, _addrInfo->ai_addrlen) == -1)
	{
		std::cerr << "Error binding socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Socket bound to port " << _listen->getPort() << std::endl;
}

/**
 * Puts the server socket into listening mode to accept incoming connections.
 */
void ServerSocket::listenSocket()
{
	if (listen(_serverSocketFd, SOMAXCONN) ==
		-1) //IMP: no se si aqui ira SOMAXCONN o un numero fijo de conexiones pendientes. SOMAXCONN es una constante que representa el número máximo de conexiones pendientes que el sistema operativo permite para un socket. Usar SOMAXCONN permite que el sistema operativo determine automáticamente un valor adecuado basado en sus propias limitaciones y configuraciones internas. Esto es útil porque garantiza que el servidor pueda manejar tantas conexiones como sea posible sin exceder los límites del sistema.
	{
		std::cerr << "Error listening on socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening for incoming connections..." << std::endl;
}

/**
 * Gets the listen configuration object associated with the server socket.
 *
 * @return A constant reference to the listen configuration object.
 */
const Listen &ServerSocket::getListen() const
{
	return *_listen;
}

/**
 * Gets the map of hostnames to configuration objects associated with the server socket.
 *
 * @return A constant reference to the map of hostnames to configuration objects.
 */
const std::map<std::string, const Config *> &ServerSocket::getConfigs() const
{
	return _configs;
}

/**
 * Gets the configuration object for a specific host associated with the server socket.
 *
 * @param host The hostname for which to retrieve the configuration.
 * @return A pointer to the configuration object for the specified host, or NULL if not found.
 */
const Config *ServerSocket::getConfigForHost(const std::string &host) const
{
	std::map<std::string, const Config *>::const_iterator it = _configs.find(host);
	if (it != _configs.end())
		return it->second;
	return NULL;
}

/**
 * Gets the default configuration object associated with the server socket.
 *
 * @return A pointer to the default configuration object, or NULL if not set.
 */
const Config *ServerSocket::getDefaultConfig() const
{
	return _defaultConfig;
}

/**
 * Gets the server socket descriptor.
 *
 * @return The server socket descriptor.
 */
int ServerSocket::getserverSocketFd() const
{
	return _serverSocketFd;
}

/**
 * Adds a configuration object for a specific host to the server socket.
 *
 * @param host The hostname for which to add the configuration.
 * @param config The configuration object to add.
 */
void ServerSocket::addConfig(const Config &config)
{
	if (!config.getServerName().empty()) //chequeamos que el server_name no este vacio, porque si lo esta no tiene sentido añadirlo al map de configs.
		_configs[config.getServerName()] = &config;
}

/**
 * Sets the default configuration object for the server socket.
 *
 * @param config The configuration object to set as the default.
 */
void ServerSocket::setDefaultConfig(const Config &config)
{
	_defaultConfig = &config;
}

/**
 * Initializes the server socket by setting up address information, creating the socket, binding it, and putting it into listening mode.
 */
void ServerSocket::startServerSocket()
{
	setupAddressInfo();
	createSocket();
	bindSocket();
	freeAddressInfo(); // Free the address info after binding the socket to avoid memory leaks
	listenSocket();
	print(); // Print the server socket information after starting it (IMP: Esto es solo para debuguear y ver que se ha creado correctamente el socket de escucha del servidor. Se puede borrar despues.)
}

/**
 * Prints the server socket information, including the port and interface it is listening on.
 */
void ServerSocket::print() const
{
	std::cout << BOLD_GREEN << "ServerSocket values:" << RESET << std::endl;
	std::cout << "  Listening on port: " << _listen->getPort() << ", Interface: " << _listen->getInterface() << std::endl;
	std::cout << "  Server socket descriptor: " << _serverSocketFd << std::endl;
	std::cout << "  Configurations for hosts:" << std::endl;
	for (std::map<std::string, const Config *>::const_iterator it = _configs.begin(); it != _configs.end(); ++it)
		std::cout << "    Host: " << it->first << " -> " << it->second << std::endl;
	std::cout << "  Default configuration: " << (_defaultConfig ? _defaultConfig->getServerName() : "None") << std::endl;
}