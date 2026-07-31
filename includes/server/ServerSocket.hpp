/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:15:20 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/31 12:52:55 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "config/Config.hpp"

class ServerSocket
{
private:
	const Listen *_listen;
	std::map<std::string, const Config *> _configs;
	const Config *_defaultConfig;
	struct addrinfo *_addrInfo;
	int _serverSocketFd;

	//Configuration and socket setup
	void setupAddressInfo();
	void freeAddressInfo();
	void createSocket();
	void bindSocket();
	void listenSocket();

public:
	ServerSocket(const Listen &listen);
	ServerSocket(const ServerSocket &other);
	ServerSocket &operator=(const ServerSocket &other);
	~ServerSocket();

	//getters
	const Listen &getListen() const;
	const std::map<std::string, const Config *> &getConfigs() const;
	const Config *getConfigForHost(const std::string &host) const;
	const Config *getDefaultConfig() const;
	int getserverSocketFd() const;

	//Setters
	void addConfig(const Config &config);
	void setDefaultConfig(const Config &config);

	void startServerSocket();
	void print() const;
};

#endif // SERVER_SOCKET_HPP