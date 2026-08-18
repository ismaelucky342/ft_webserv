#ifndef SERVER_HPP
#define SERVER_HPP

#include <poll.h>
#include <vector>
#include <map>
#include <string>
#include "config/Config.hpp" //IMP: revisar si sobra algun include.
#include "http/HTTPResponse.hpp"
#include "http/HTTPRequest.hpp"
#include "http/HTTPRequestParser.hpp"
#include "http/HTTPStatus.hpp"
#include "server/Client.hpp"
#include "server/ServerSocket.hpp"
class Server
{
private:
	std::vector<ServerSocket> _serverSockets;
	std::vector<struct pollfd> _pollFds;
	std::map<int, Client> _clients;
	HTTPRequestParser _parser;

	void initPoll();
	void runLoop();
	void setPollEvent(int clientSocket, short events);
	//Client handling
	void acceptClient(ServerSocket &serverSocket);
	void readFromClient(int clientSocket);
	void writeToClient(int clientSocket);
	Client &getClient(int clientSocket);
	void disconnectClient(int clientSocket);
	//Request handling
	const Config *isValidConfig(const Config *config, int clientSocket);
	const Config *getConfigFromHost(const std::string &host, const ServerSocket &serverSocket, int clientSocket);
	HTTPResponse handleRequest(const HTTPRequest &request, const Config *config);
	//Response handling
	HTTPResponse createResponse(HTTPStatus statusCode, const std::string &contentType, const std::string &body);
	HTTPResponse createErrorResponse(HTTPStatus statusCode, const Config *config);
	HTTPResponse createDefaultErrorPage(HTTPStatus statusCode);
	//Vector of server sockets handling
	ServerSocket *getServerSocketByFd(int fd);
	ServerSocket *findServerSocket(const Listen &listen);

public:
	Server(const std::vector<Config> &configs);
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	void start();
};

#endif