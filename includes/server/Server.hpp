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
#include "http/HTTPException.hpp"
#include "server/Client.hpp"
#include "server/ServerSocket.hpp"
#include "config/EffectiveConfig.hpp"
class Server
{
private:
	std::vector<ServerSocket> _serverSockets;
	std::vector<struct pollfd> _pollFds;
	std::map<int, Client> _clients;
	HTTPRequestParser _parser;

	Server(const Server &other); // Lo ponemos en privado para que no se pueda usar, ya que no tiene sentido tener dos servers con los mismos fds y la misma configuracion. Si se quiere duplicar un server hay que crear uno nuevo con el constructor de copia, que no copia los fds ni la configuracion, sino que crea un nuevo server con la misma configuracion.
	Server &operator=(const Server &other); // Lo ponemos en privado para que no se pueda usar, ya que no tiene sentido tener dos servers con los mismos fds y la misma configuracion. Si se quiere duplicar un server hay que crear uno nuevo con el constructor de copia, que no copia los fds ni la configuracion, sino que crea un nuevo server con la misma configuracion.

	void initPoll();
	void runLoop();
	void setPollEvent(int clientSocket, short events);
	void printPollFds() const; // IMP: Para depuracion y entender mejor. Muestra por pantalla los fds que estamos escuchando y sus eventos.
	//Client handling
	void acceptClient(ServerSocket &serverSocket);
	bool readFromClient(int clientSocket);
	bool writeToClient(int clientSocket);
	Client &getClient(int clientSocket);
	void disconnectClient(size_t index, bool doClose);
	void disconnectClientByFd(int clientSocketFd, bool doClose);
	void cleanupDisconnectedClients();
	void updateClientKeepAlive(Client &client);
	void checkClientTimeouts();
	//Request handling
	bool processNextRequest(int clientSocket);
	bool processChunkedBody(Client &client, const Config *config, const size_t &headersEnd, size_t &requestEnd, std::string &body);
	bool processContentLengthBody(Client &client, const Config *config, const size_t &headersEnd, size_t &requestEnd, std::string &body);
	const Config *isValidConfig(const Config *config, int clientSocket);
	const Config *getConfigFromHost(const std::string &host, const ServerSocket &serverSocket, int clientSocket);
	EffectiveConfig resolveConfig(const Config *config, const Location *location) const;
	HTTPResponse handleRequest(const HTTPRequest &request, const Config *config, int &fileFd, size_t &fileSize);
	bool isMethodAllowed(const HTTPRequest &request, const Location *location);
	//Response handling
	HTTPResponse createResponse(HTTPStatus statusCode, const std::string &contentType, const std::string &body);
	HTTPResponse createErrorResponse(const HTTPException &e, const Config *config, int &fileFd, size_t &fileSize);
	HTTPResponse createDefaultErrorPage(HTTPStatus statusCode);
	HTTPResponse createRedirectResponse(HTTPStatus statusCode, const std::string &redirectTarget);
	HTTPResponse createAutoIndexResponse(const std::string &requestPath);
	HTTPResponse createFileResponse(const std::string &srcPath, size_t fileSize, int &fileFd);
	void		 prepareResponseForSending(Client &client, const HTTPResponse &response, int &fileFd, size_t fileSize);
	//Vector of server sockets handling
	ServerSocket *getServerSocketByFd(int fd);
	bool		 sameInterface(const std::string &interface1, const std::string &interface2) const;
	ServerSocket *findServerSocket(const Listen &listen);
	// Other private methods for server functionality can be added here
	std::string getContentType(const std::string &path) const;

public:
	Server(const std::vector<Config> &configs);
	// Server(const Server &other);
	// Server &operator=(const Server &other);
	~Server();

	void start();
};

#endif