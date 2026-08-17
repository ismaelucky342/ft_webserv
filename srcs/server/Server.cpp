#include <unistd.h> // close()
#include <iostream>
#include <fstream>		// std::ifstream
#include <sstream>		// std::ostringstream
#include <sys/socket.h> // socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h> // sockaddr_in, htons(), ntohs(), INADDR_ANY

#include "server/Server.hpp"
#include <arpa/inet.h> // inet_ntoa(), inet_ntop(), inet_pton(), inet_addr()
#include "http/HTTPException.hpp"

/**
 * Constructor for the Server class.
 * 
 * @param config The configuration object containing server settings.
 */
Server::Server(const std::vector<Config> &configs)
{
	for (size_t i = 0; i < configs.size(); ++i)
	{
		const Config &config = configs[i];
		const std::vector<Listen> &confListens = config.getListens();

		for (size_t j = 0; j < confListens.size(); ++j)
		{
			const Listen &listen = confListens[j];
			ServerSocket *serverSocket = findServerSocket(listen);
			if (serverSocket == NULL) // No existe ningún ServerSocket compatible, así que creamos uno nuevo.
			{
				_serverSockets.push_back(ServerSocket(listen));
				serverSocket = &_serverSockets.back();
				serverSocket->setDefaultConfig(config); // El primero es el default provisional.
			}
			else
			{
				// Si el nuevo listen es 0.0.0.0 y el existente
				// era una interfaz concreta, el wildcard tiene prioridad.
				if (listen.getInterface() == "0.0.0.0" &&
					serverSocket->getListen().getInterface() != "0.0.0.0")
				{
					serverSocket->setListen(listen); // Actualizamos el listen del ServerSocket existente al nuevo listen con la interfaz "0.0.0.0"
					serverSocket->setDefaultConfig(config); // el config por defecto ahora es el del listen con la interfaz "0.0.0.0"
				}
			}
			serverSocket->addConfig(config); // Añadimos la configuración al ServerSocket.

		}
	}
}

/**
 * Copy constructor for the Server class.
 * 
 * @param other The Server object to copy from.
 */
Server::Server(const Server &other)
	: _serverSockets(other._serverSockets), _pollFds(other._pollFds), _clients(other._clients),
	  _parser(other._parser)
{
	std::cout << BOLD_GREEN << "Server copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the Server class.
 * 
 * @param other The Server object to assign from.
 * @return A reference to the assigned Server object.
 */
Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		_serverSockets = other._serverSockets;
		_pollFds = other._pollFds;
		_clients = other._clients;
		_parser = other._parser;
	}
	std::cout << BOLD_GREEN << "Server assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the Server class.
 */
Server::~Server()
{
	std::cout << BOLD_RED << "Server destructor called" << RESET << std::endl;
}

/**
 * Starts the server and enters the main loop to handle incoming connections and requests.
 */
void Server::start()
{
	for (size_t i = 0; i < _serverSockets.size(); ++i)
		_serverSockets[i]
			.startServerSocket();

	initPoll();
	runLoop();
}

/**
 * Initializes the poll structure to monitor the all the server sockets for incoming connections.
 */
void Server::initPoll()
{
	for (size_t i = 0; i < _serverSockets.size(); ++i)
	{
		struct pollfd
			serverPoll; //Un pollfd es una estructura (ya existente en la libreria poll.h) que contiene un descriptor de archivo (fd), los eventos que queremos escuchar (events) y los eventos que han ocurrido (revents).
		serverPoll.fd =
			_serverSockets[i]
				.getserverSocketFd(); // Obtenemos el descriptor de archivo del socket del servidor que hemos creado en la clase ServerSocket. Este es el socket que escuchará nuevas conexiones entrantes.
		serverPoll.events =
			POLLIN; // En este caso, vamos a escuchar el socket del servidor para ver si hay nuevas conexiones entrantes (POLLIN). Solo se activara si hay una nueva conexion entrante, no si hay datos para leer de un cliente ya conectado.
		serverPoll.revents =
			0; // Revents se inicializa a 0 porque aún no han ocurrido eventos. El kernel marcará el revents con el evento que haya ocurrido (si es que ocurre alguno) cuando llamemos a poll().

		_pollFds.push_back(
			serverPoll); // Agregamos el pollfd del servidor al vector de pollfd que vamos a pasar a la función poll(). De esta manera, el servidor estará escuchando nuevas conexiones entrantes en su socket.
		// El vector _pollFds contendrá todos los pollfd que queremos escuchar: el del servidor y los de los clientes conectados. Cuando llamemos a poll(), el kernel nos dirá cuáles de estos pollfd tienen eventos pendientes (nuevas conexiones o datos para leer).
		// Es como una lista de elementos a los que el poll estará atento y nos avisará cuando haya algo que atender.
	}
}

/**
 * Main loop of the server that waits for events on the monitored file descriptors and handles them accordingly.
 */
void Server::runLoop()
{
	while (true)
	{
		int pollCount = poll(
			_pollFds.data(), _pollFds.size(),
			-1); //Activamos el Poll para que empiece a escuchar los eventos. Parametros: array de pollfd, número de fds, timeout (-1 = infinito)
		if (pollCount == -1)
		{
			std::cerr << "Error in poll()" << std::endl;
			continue;
		}
		for (
			size_t i = 0; i < _pollFds.size();
			++i) //Cuando ocurre un evento en cualquiera de los elementos en los que esta escuchando el poll, se sale y tenemos que recorrerlos con el bucle pasando por todos (incluso los que no tienen eventos activos en su revent.)
		{
			struct pollfd &pfd = _pollFds[i];
			ServerSocket *serverSocket = getServerSocketByFd(pfd.fd);

			if (serverSocket !=
				NULL) //Si el elemento es uno de los sockets de servidor, entonces es una nueva conexion entrante y tenemos que aceptarla. Si no es un socket de servidor, entonces es un socket de cliente y tenemos que leer o escribir en el.
			{
				if (pfd.revents &
					POLLIN) // Si el elemento es el propio servidor y tiene un evento pendiente (revents != 0) y ese evento es POLLIN (una nueva conexión entrante). EL & es un and bit a bit, porque el revent se maneja por bits para cada tipo de evento. Si el revent tiene el bit de POLLIN activado, significa que hay datos para leer o una nueva conexión entrante. Por eso se hace en & con el bit del POLLIN que nos asegura que, al menos el bit del POLLIN esta activo.
				{
					acceptClient(*serverSocket);
					continue;
				}
			}
			if (pfd.revents &
				(POLLHUP | POLLERR |
				 POLLNVAL)) // Si en el socket que estamos atendiendo hay un error (POLLERR), el socket se ha cerrado (POLLHUP) o el descriptor de archivo no es válido (POLLNVAL)...
			{
				if (getServerSocketByFd(pfd.fd) !=
					NULL) // Si el error ocurre en un socket de servidor, significa que hay un problema con el socket de escucha del servidor. Esto es crítico porque el servidor no podrá aceptar nuevas conexiones. En este caso, lanzamos una runtime_error para indicar que hay un error en el socket, esta llegaria al main y saldriamos del programa.
					throw std::runtime_error("Error on listening socket");
				disconnectClient(
					pfd.fd); // Si el error ocurre en un socket de cliente, desconectamos al cliente y seguimos con el bucle. Esto es importante porque si no hacemos esto, el poll se quedaria bloqueado esperando a que ocurra un evento en ese socket que ya no es valido y el servidor se quedaria colgado.
				continue;
			}
			if (pfd.revents &
				POLLIN) //  El socket del cliente tiene datos disponibles para ser leídos mediante recv().
				readFromClient(pfd.fd);
			if (pfd.revents &
				POLLOUT) // El socket está listo para enviar datos al cliente mediante send().
				writeToClient(pfd.fd);
		}
	}
}

/**
 * Sets the event for a client socket in the poll structure.
 * 
 * clientSocket: The socket connected to the client.
 * events: The events to monitor.
 */
void Server::setPollEvent(int clientSocket, short events)
{
	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].fd == clientSocket)
		{
			_pollFds[i].events = events;
			return;
		}
	}
}

/**
 * Accepts a new client connection and adds it to the poll structure for monitoring.
 */
void Server::acceptClient(ServerSocket &serverSocket)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int clientSocket =
		accept(serverSocket.getserverSocketFd(), (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientSocket == -1)
	{
		std::cerr << "Error accepting client connection" << std::endl;
		return;
	}
	std::cout
		<< "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":"
		<< ntohs(clientAddr.sin_port)
		<< std::
			   endl; //IMP: Para depuracion y entender mejor. inet_ntoa convierte la dirección IP del cliente de formato binario a formato de cadena legible. ntohs convierte el puerto del cliente de orden de bytes de red a orden de bytes del host para que se pueda mostrar correctamente.

	struct pollfd
		clientPoll; //Creamos un nuevo pollfd para el cliente que acabamos de aceptar. Este pollfd nos permitirá escuchar los eventos que ocurran en el socket del cliente, como datos entrantes (POLLIN) o desconexión (POLLHUP).

	clientPoll.fd = clientSocket;
	clientPoll.events = POLLIN; // Wait for incoming data from the client
	clientPoll.revents = 0;

	_pollFds.push_back(clientPoll); // lo añadimos en el vector de elementos a escuchar.
	_clients.insert(std::make_pair(clientSocket, Client(clientSocket, serverSocket)));
}

/**
 * Reads data from a client socket, parses the HTTP request, and prepares the response.
 * 
 * clientSocket: The socket connected to the client.
 */
void Server::readFromClient(int clientSocket)
{
	char buffer[4096];
	Client &client = getClient(clientSocket);
	HTTPRequestParser parser;

	ssize_t bytes =
		recv(clientSocket, buffer, sizeof(buffer) - 1, 0); //Leemos lo que nos envia el cliente
	// bytes > 0: data received, bytes == 0: client disconnected in a good way, bytes < 0: error
	if (bytes == 0)
	{
		std::cout << "Client disconnected." << std::endl;
		disconnectClient(clientSocket);
		return;
	}
	if (bytes < 0)
	{
		std::cerr << "Error receiving data from client." << std::endl;
		disconnectClient(clientSocket);
		return;
	}
	buffer[bytes] = '\0';
	std::cout
		<< "Request: " << buffer
		<< std::
			   endl; //Linea para mostrar por pantalla la peticion y poder entenderla. IMP: luego podemos quitarla.
	client.getRecvBuffer().append(
		buffer,
		bytes); //Añadimos lo que hemos leido al buffer de recepcion del cliente. Esto es importante porque el cliente puede enviar la peticion en varios paquetes y tenemos que ir acumulando todo hasta tener la peticion completa.

	if (!parser.isRequestComplete(
			client
				.getRecvBuffer())) //Si la peticion no esta completa, salimos y esperamos a que llegue el resto de la peticion. Esto es importante porque el cliente puede enviar la peticion en varios paquetes y tenemos que ir acumulando todo hasta tener la peticion completa.
	{
		std::cout << "Request not complete yet." << std::endl;
		return;
	}
	std::cout << "Request complete." << std::endl;
	std::cout
		<< "Imprimiendo request: " << std::endl << client.getRecvBuffer() << std::endl; //Linea para mostrar por pantalla la peticion y poder entenderla. IMP: luego podemos quitarla.
	try
	{
		HTTPRequest request = parser.parse(
			client
				.getRecvBuffer()); //Parseamos la peticion y obtenemos un objeto HTTPRequest con los datos parseados. Esto implica leer la request line, los headers y el body de la peticion.
		request.print(); // IMP LUEGO BORRRAR:Mostramos por pantalla los datos parseados de la peticion para poder entenderla. Esto es importante para depurar y entender mejor la peticion que nos envia el cliente.

		client.getResponse() = handleRequest(
			request,
			client.getServerSocket(),
			client.getFd()); //Manejamos la peticion y generamos la respuesta correspondiente. Esto implica leer el fichero solicitado, generar la cabecera de la respuesta y el cuerpo de la respuesta.
		client.getSendBuffer() =
			client.getResponse()
				.serialize(); //Serializamos la respuesta y la añadimos al buffer de envio del cliente. Esto es importante porque el cliente puede enviar la respuesta en varios paquetes y tenemos que ir enviando todo hasta que se haya enviado toda la respuesta.
		setPollEvent(
			clientSocket,
			POLLOUT); // Cambiamos el evento a POLLOUT para que el poll nos avise cuando el socket del cliente esté listo para que le enviemos datos.
	}
	catch (const HTTPException &e)
	{
		client.getResponse() = createErrorResponse(
			static_cast<HTTPStatus>(e.getStatusCode()),
			client
				.getServerSocket()); // Si ocurre una excepción HTTP (por ejemplo, un error de parseo de la solicitud), generamos una respuesta de error correspondiente y la enviamos al cliente. Luego tendremos que mejorarlo con las paginas de error personalizadas que nos indicara en archivo de configuracion.
		client.getSendBuffer() = client.getResponse().serialize();
		setPollEvent(clientSocket, POLLOUT);
		return;
	}
}

void Server::writeToClient(int clientSocket)
{
	Client &client = getClient(clientSocket);
	std::string &sendBuffer = client.getSendBuffer();

	ssize_t bytesSent = send(clientSocket, sendBuffer.c_str() + client.getBytesSent(),
							 sendBuffer.size() - client.getBytesSent(), 0);
	if (bytesSent < 0)
	{
		disconnectClient(clientSocket);
		return;
	}
	client.addBytesSent(bytesSent);
	std::cout << "Sent " << bytesSent << " total to " << sendBuffer.size() << " bytes to client."
			  << std::endl;
	if (client.getBytesSent() < sendBuffer.size())
	{
		std::cout << "Response not fully sended yet." << std::endl;
		return;
	}
	if (client.isKeepAlive())
	{
		client.reset();
		setPollEvent(clientSocket, POLLIN);
	}
	else
		disconnectClient(clientSocket);
}

/**
 * Gets a client from map of clients in the server.
 * 
 * @param clientSocket The socket connected to the client.
 * @return A reference to the client object.
 */
Client &Server::getClient(int clientSocket)
{
	std::map<int, Client>::iterator it = _clients.find(clientSocket);
	if (it == _clients.end())
		throw std::runtime_error("Client not found");
	return it->second;
}

/**
 * Disconnects a client from the server, closing the socket and removing it from the poll structure.
 * 
 * @param clientSocket The socket connected to the client.
 */
void Server::disconnectClient(int clientSocket)
{
	close(clientSocket);
	for (size_t i = 0; i < _pollFds.size(); ++i)
	{
		if (_pollFds[i].fd == clientSocket)
		{
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}
	_clients.erase(clientSocket);
}

/**
 * Validates a configuration for a specific client checking the local address and port of the request.
 * 
 * @param config The configuration to validate.
 * @param clientSocket The socket connected to the client.
 * @return A pointer to the valid configuration object, or NULL if invalid.
 */
const Config *Server::isValidConfig(const Config *config, int clientSocket)
{
	struct sockaddr_in localAddr;
	socklen_t addrLen = sizeof(localAddr);
	if (getsockname(clientSocket, (struct sockaddr *)&localAddr, &addrLen)< 0)
		throw HTTPException(INTERNAL_SERVER_ERROR);
	std::string localInterface = inet_ntoa(localAddr.sin_addr);
	int localPort = ntohs(localAddr.sin_port);
	std::cout << "---------------------------------------------Local interface: " << localInterface << ", Local port: " << localPort << std::endl; //IMP: PARA DEBUG borrar luego.

	const std::vector<Listen>& listens = config->getListens();
	for (size_t i = 0; i < listens.size(); ++i)
	{
		const Listen &listen = listens[i];
		if (listen.getPort() != localPort)
			continue;
		if (listen.getInterface() == "0.0.0.0" || listen.getInterface() == localInterface)
			return config;
	}
	return NULL;
}

/**
 * Gets the configuration for a specific host from the server socket.
 * 
 * @param host The host name to look for.
 * @param serverSocket The server socket to search in.
 * @return A pointer to the configuration object, or NULL if not found.
 */
const Config *Server::getConfigFromHost(const std::string &host, const ServerSocket &serverSocket, int clientSocket)// IMP Revisar hecho rapido AQUIIIIII
{
	const Config *config = NULL;
	if (!host.empty())
	{	
		size_t colonPos = host.find(':');
		if (colonPos == std::string::npos)
			throw HTTPException(BAD_REQUEST);
		std::string interface = host.substr(0, colonPos);
		config = serverSocket.getConfigForHost(interface);
	}
	if (config == NULL)
		config = serverSocket.getDefaultConfig();
	return isValidConfig(config, clientSocket);
}

/**
 * Handles an HTTP request and generates an appropriate response.
 * 
 * @param request The HTTP request to handle.
 * @return The HTTP response to send.
 */
HTTPResponse Server::handleRequest(const HTTPRequest &request, const ServerSocket &serverSocket, int clientSocket)
{
	std::string srcPath;
	const Config *config = getConfigFromHost(request.getHeader("Host"), serverSocket, clientSocket);
	if (config == NULL)
		throw HTTPException(FORBIDDEN); // Si no hay un config valido para el host, devolvemos un error 403 Forbidden. Esto es importante porque si no hay un config valido, significa que el host no esta permitido y no podemos servir la peticion.

	if (request.getPath() ==
		"/") // calcula donde esta la pagina html a decolver segun los parametros parseados del archivo conf.
		srcPath = config->getRoot() + "/" + config->getIndex(); // el index por defecto
	else
		srcPath = config->getRoot() + request.getPath(); // la pagina solicitada

	std::ifstream file_stream(srcPath.c_str());
	if (!file_stream.is_open()) // si no puede abrir el fichero o no existe, devolvemos un error 404
		return createErrorResponse(NOT_FOUND, serverSocket);
	else // si puede abrir el fichero, lo leemos y lo devolvemos como respuesta.
	{
		std::ostringstream bodystream;
		bodystream << file_stream.rdbuf();
		std::string body = bodystream.str();
		return createResponse(OK, "text/html", body);
	}
}

/**
 * Creates an HTTP response with the specified status code, content type, and body.
 * 
 * @param statusCode The HTTP status code.
 * @param contentType The content type of the response.
 * @param body The body content of the response.
 * @return The created HTTP response.
 */
HTTPResponse Server::createResponse(HTTPStatus statusCode, const std::string &contentType,
									const std::string &body)
{
	HTTPResponse response;
	std::string statusMessage = getStatusMessage(statusCode);

	std::ostringstream lengthstream;
	lengthstream << body.length();

	response.setStatusCode(statusCode);
	response.setStatusMessage(statusMessage);
	response.setHeader("Content-Type", contentType);
	response.setHeader("Content-Length", lengthstream.str());
	response.setBody(body);
	return response;
}

/**
 * Creates an error HTTP response with the specified status code.
 * 
 * @param statusCode The HTTP status code.
 * @return The created HTTP response.
 */
HTTPResponse Server::createErrorResponse(HTTPStatus statusCode, const ServerSocket &serverSocket)
{
	const Config &config = *serverSocket.getDefaultConfig(); //ATENCION SOLO PARA PROBAR: Obtenemos la configuracion por defecto del servidor que ha recibido la peticion. Esto es importante porque la configuracion contiene el root y el index que necesitamos para calcular la ruta del fichero a devolver.
	std::ostringstream errorPagePathStream;

	errorPagePathStream << config.getRoot() << config.getErrorPage(statusCode);
	std::ostringstream bodystream;

	std::ifstream error_file_stream(errorPagePathStream.str().c_str());
	if (!error_file_stream
			 .is_open()) // si no puede abrir el fichero o no existe, devolvemos un error 404
	{
		bodystream << statusCode << " " << getStatusMessage(statusCode);
		std::string body = bodystream.str();
		return createResponse(statusCode, "text/plain", body);
	}
	else // si puede abrir el fichero, lo leemos y lo devolvemos como respuesta.
	{
		bodystream << error_file_stream.rdbuf();
		std::string body = bodystream.str();
		return createResponse(statusCode, "text/html", body);
	}
}

/**
 * Gets the server socket object corresponding to the given file descriptor.
 * 
 * @param fd The file descriptor of the server socket.
 * @return A pointer to the ServerSocket object, or NULL if not found.
 */
ServerSocket *Server::getServerSocketByFd(
	int fd) //devolvemos un puntero porque asi podemos devolver NULL si no encontramos el socket, y si devolvieramos una referencia no podriamos devolver NULL.
{
	for (size_t i = 0; i < _serverSockets.size(); ++i)
	{
		if (_serverSockets[i].getserverSocketFd() == fd)
			return &_serverSockets[i];
	}
	return NULL;
}

/**
 * Finds a server socket corresponding to the given listen configuration.
 * 
 * @param listen The listen configuration to search for.
 * @return A pointer to the ServerSocket object, or NULL if not found.
 */
ServerSocket *Server::findServerSocket(const Listen &listen)
{
	for (size_t i = 0; i < _serverSockets.size(); ++i)
	{
		const Listen &serverSocketListen = _serverSockets[i].getListen();

		// Si el puerto es diferente, no puede ser el mismo socket.
		if (listen.getPort() != serverSocketListen.getPort())
			continue;

		// Misma interfaz y mismo puerto.
		if (listen.getInterface() == serverSocketListen.getInterface())
			return &_serverSockets[i];

		// Si ambos tienen el mismo puerto pero uno de los dos es interfaz 0.0.0.0, ese pasa a será el devuelto para modificarlo y hacerlo el serversocket con cubra a los demoas con ese puerto.
		// Si listen.getInterface() == "0.0.0.0" el que devolvemos será el que modificaremos.
		// Si serverSocketListen.getInterface() == "0.0.0.0" el que devolvemos sólo le añadiremos el nuevo config al map porque significa que el 0.0.0.0 ya existía.
		if (listen.getInterface() == "0.0.0.0" ||
			serverSocketListen.getInterface() == "0.0.0.0")
			return &_serverSockets[i];
	}

	return NULL;
}