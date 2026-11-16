/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:15:20 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/16 16:20:27 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "config/Config.hpp"

class ServerSocket
{
private:
	Listen _listen;
	std::map<std::string, const Config *> _configs;
	const Config *_defaultConfig;
	struct addrinfo *_addrInfo;
	int _serverSocketFd;

	ServerSocket &operator=(const ServerSocket &other); //Asignación lo ponemos en privado para que no se pueda usar, ya que no tiene sentido tener dos serversockets con el mismo fd y la misma direccion. Si se quiere duplicar un serversocket hay que crear uno nuevo con el constructor de copia, que no copia el fd ni la direccion, sino que crea un nuevo serversocket con la misma configuracion.

	//Configuration and socket setup
	void setupAddressInfo();
	void freeAddressInfo();
	void createSocket();
	void bindSocket();
	void listenSocket();

public:
	ServerSocket(const Listen &listen); //Lo tenemos en publico porque es necesario para introducirlos en el std::vector<ServerSocket> _serverSockets de server. Aqui no hay problema de usarlo porque el fd y el addrinfo todavía no han sido creados/tienen sus valores por defecto). Pero una vez creado el serversocket no tiene sentido utilizarlo. Tenerlo en cuenta! 
	ServerSocket(const ServerSocket &other);
	~ServerSocket();

	//getters
	const Listen &getListen() const;
	const std::map<std::string, const Config *> &getConfigs() const;
	const Config *getConfigFromKey(const std::string &key) const;
	const Config *getDefaultConfig() const;
	int getserverSocketFd() const;

	//Setters
	void setListen(const Listen &listen);
	void addConfig(const Config &config);
	void setDefaultConfig(const Config &config);

	void startServerSocket();
	void print() const;
};

#endif // SERVER_SOCKET_HPP