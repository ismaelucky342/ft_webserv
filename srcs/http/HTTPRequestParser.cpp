#include "http/HTTPRequestParser.hpp"
#include <iostream>
#include <sstream>
#include "colors.hpp"
#include "http/HTTPException.hpp"
#include "http/HTTPStatus.hpp"
#include "StringUtils.hpp"
#include "limits"

/**
 * Default constructor for the HTTPRequestParser class.
 */
HTTPRequestParser::HTTPRequestParser()
{
	std::cout << BOLD_GREEN << "HTTPRequestParser default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the HTTPRequestParser class.
 * 
 * other: The HTTPRequestParser object to copy.
 */
HTTPRequestParser::HTTPRequestParser(const HTTPRequestParser &other)
{
	(void)other;
	std::cout << BOLD_GREEN << "HTTPRequestParser copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the HTTPRequestParser class.
 * 
 * other: The HTTPRequestParser object to assign.
 * @return A reference to the assigned object.
 */
HTTPRequestParser &HTTPRequestParser::operator=(const HTTPRequestParser &other)
{
	(void)other;
	std::cout << BOLD_GREEN << "HTTPRequestParser assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the HTTPRequestParser class.
 */
HTTPRequestParser::~HTTPRequestParser()
{
	std::cout << BOLD_RED << "HTTPRequestParser destructor called" << RESET << std::endl;
}

/**
 * Parses the raw HTTP request string and returns an HTTPRequest object.
 * 
 * rawRequest: The raw HTTP request string to parse.
 * @return An HTTPRequest object initialized with the parsed values.
 */
HTTPRequest HTTPRequestParser::parse(const std::string &rawRequest)
{
	HTTPRequest request;
	parseRequestLine(request, rawRequest);
	parseHeaders(request, rawRequest);
	//IMP: Implement parseBody methods to handle headers and body parsing. No hace falta porque se asigna directamente.
	return request;
}

/**
 * Parses the request line of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * rawRequest: The raw HTTP request string.
 */
void HTTPRequestParser::parseRequestLine(HTTPRequest &request, const std::string &rawRequest)
{
	if (request.isRequestLineParsed())
		return ; // Si la request line ya ha sido parseada, no hacemos nada.

	std::istringstream rawStream(rawRequest);
	std::string requestLine;

	if (!std::getline(rawStream, requestLine))
		throw HTTPException(BAD_REQUEST, true);

	std::istringstream requestLineStream(requestLine);
	std::string method, path, version, extra;

	if (!(requestLineStream >> method >> path >>
		  version)) // Intenta leer exactamente los tres campos de la request line. Si alguna lectura falla, el stream entra en estado de error.
		throw HTTPException(BAD_REQUEST, true);
	if (requestLineStream >> extra) //si hay más datos en el stream, es un error
		throw HTTPException(BAD_REQUEST, true);

	parseMethod(request, method);
	parsePath(request, path);
	parseVersion(request, version);
	request.setRequestLineParsed(true); // Marcamos que la request line ha sido parseada correctamente.
}

/**
 * Parses the headers of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * rawRequest: The raw HTTP request string.
 */
void HTTPRequestParser::parseHeaders(HTTPRequest &request, const std::string &rawRequest) //IMPORTANTE Mejorar el parseo aqui también. de momento para probar bien.
{
	if (request.areHeadersParsed())
		return ; // Si los headers ya han sido parseados, no hacemos nada.

	std::istringstream rawStream(rawRequest);
	std::string requestLine, headerLine, key, value;

	if (!std::getline(rawStream, requestLine))
		throw HTTPException(BAD_REQUEST, true);

	while (std::getline(rawStream, headerLine) && headerLine != "\r")
	{
		// getline() elimina '\n', pero deja '\r'
		if (!headerLine.empty() && headerLine[headerLine.size() - 1] == '\r') // Asi que sin no esta vacio y el ultimo caracter es '\r', lo eliminamos.
			headerLine.erase(headerLine.size() - 1); //erase elimina el caracter en la posicion indicada, que es la ultima posicion del string.

		parseHeaderLine(headerLine, key, value); //IMPORTANTE hacer la comprobacion de headers repetidas
		request.setHeader(StringUtils::toLowerString(key), value);
	}
	request.setHeadersParsed(true); // Marcamos que los headers han sido parseados correctamente.
}

/**
 * Parses the body of an HTTP request with a Content-Length header.
 *
 * @param buffer The buffer containing the raw HTTP request string.
 * @param bodyStart The position in the buffer where the body starts.
 * @param bodyLength The length of the body.
 * @param requestEnd The position in the buffer where the request ends. It will be updated by this function.
 * @param body The parsed body string. It will be updated by this function.
 * @return True if the body was parsed successfully, false if the request is incomplete or invalid.
 */
bool HTTPRequestParser::parseContentLengthBody(const std::string &buffer, size_t bodyStart, size_t bodyLength, size_t &requestEnd, std::string &body)
{
	requestEnd = getContentLengthRequestEnd(buffer, bodyStart, bodyLength); //Obtenemos la posición del final de la petición completa (headers + body) en el buffer de recepción del cliente.
	if (requestEnd == std::string::npos) //Si no hemos encontrado el final de una peticion con body, salimos y esperamos a que llegue el resto de la peticion. Esto es importante porque el cliente puede enviar la peticion en varios paquetes y tenemos que ir acumulando todo hasta tener la peticion completa.
	{
		std::cout << "Request length not complete yet." << std::endl;
		return false;
	}
	body = buffer.substr(bodyStart, requestEnd - bodyStart); //Obtenemos la subcadena del body de la peticion, ya que hemos comprobado que hemos recibido todo el body.
	return true;
}

/**
 * Parses a chunked body of an HTTP request.
 * 
 * buffer: The buffer containing the raw HTTP request string.
 * bodyStart: The position in the buffer where the body starts.
 * maxBodySize: The maximum allowed size of the body. If the parsed body exceeds this size, an exception will be thrown.
 * requestEnd: The position in the buffer where the request ends. It will be updated by this function.
 * body: The parsed body string. It will be updated by this function.
 * @return True if the body was parsed successfully, false if the request is incomplete or invalid. It throws an HTTPException if the request is invalid.
 * @throws HTTPException if the request is malformed.
 */
bool HTTPRequestParser::parseChunkedBody(const std::string &buffer, size_t bodyStart, const size_t maxBodySize, size_t &requestEnd, std::string &body)
{
	requestEnd = getChunkedRequestEnd(buffer, bodyStart, maxBodySize); //Obtenemos la posición del final de la petición completa (headers + body) en el buffer de recepción del cliente. Aqui buscamos el 
	if (requestEnd == std::string::npos) //Si no hemos encontrado el final de una peticion con body, salimos y esperamos a que llegue el resto de la peticion. Esto es importante porque el cliente puede enviar la peticion en varios paquetes y tenemos que ir acumulando todo hasta tener la peticion completa.
	{
		std::cout << "Request chunked not complete yet." << std::endl;
		return false;
	}
	std::string rawBody = buffer.substr(bodyStart, requestEnd - bodyStart); //Obtenemos la subcadena del body de la peticion, ya que hemos comprobado que hemos recibido todo el body. Y lo guardamos/parseamos en el objeto HTTPRequest del cliente.
	body = cleanChunkedBody(rawBody); //Parseamos el body de la peticion y lo guardamos en el objeto HTTPRequest del cliente. (el body es un string).
	return true;
}

/**
 * Parses the normalized body of the HTTP request after it's been processed through Chunked or content-length parsing.
 * 
 * request: The HTTPRequest object to populate.
 * body: The body string to parse.
 */
void HTTPRequestParser::parseNormalizedBody(HTTPRequest& request, const std::string& body)
{
	if (request.isBodyParsed())
		return ; // Si el body ya ha sido parseado, no hacemos nada.

	request.setBody(body);
	request.setBodyParsed(true); // Marcamos que el body ha sido parseado correctamente.
}

/**
 * Gets the position of the end of the headers in a HTTP request.
 * 
 * buffer: The buffer containing the raw HTTP request string.
 * @return The position of the end of the first found request.
 */
size_t HTTPRequestParser::getHeadersEnd(const std::string &buffer)
{
	size_t pos = buffer.find("\r\n\r\n");

	if (pos == std::string::npos)
		return std::string::npos;

	return pos + 4;
}

/**
 * Gets the position of the end of a HTTP request based on the body start and header content-length.
 * 
 * buffer: The buffer containing the raw HTTP request string.
 * bodyStart: The position in the buffer where the body starts.
 * bodyLength: The length of the body.
 * @return The position of the end of the request, or std::string::npos if the request is incomplete.
 */
size_t HTTPRequestParser::getContentLengthRequestEnd(const std::string &buffer, size_t bodyStart, size_t bodyLength)
{
	size_t maxValue = std::numeric_limits<size_t>::max();

	if (bodyStart > maxValue - bodyLength) // Si supera el max de un size_t. truco: Lo compruebo representandolo con la resta (despejando la ecuación) para evitar el overflow en la suma
		throw HTTPException(BAD_REQUEST, true);

	size_t requestEnd = bodyStart + bodyLength;

	if (buffer.size() < requestEnd)
		return std::string::npos;

	return requestEnd;
}

/**
 * Gets the position of the end of a chunked HTTP request.
 *
 * buffer: The buffer containing the raw HTTP request string.
 * bodyStart: The position in the buffer where the body starts.
 * @return The position of the end of the request, or std::string::npos if the request is incomplete.
 * @throws HTTPException if the request is malformed.
 */
size_t HTTPRequestParser::getChunkedRequestEnd(const std::string &buffer, size_t bodyStart, const size_t maxBodySize)
{
	size_t pos = bodyStart;
	size_t totalBodySize = 0;
	size_t maxValue = std::numeric_limits<size_t>::max();
	while (true)  //Ejemplo de body chunked: 5\r\nhello\r\n0\r\n\r\n
	{
		size_t lineEnd = buffer.find("\r\n", pos);
		if (lineEnd == std::string::npos)
			return std::string::npos;
		std::string chunkSizeStr = buffer.substr(pos, lineEnd - pos);
		size_t chunkSize = 0;
		if (!StringUtils::parseSizeT(chunkSizeStr, chunkSize, 16)) //Parseamos el tamaño del chunk en hexadecimal. Si no se puede parsear, lanzamos una excepción
			throw HTTPException(BAD_REQUEST, true);
		pos = lineEnd + 2;
		if (chunkSize == 0) // Si el tamaño del chunk es 0, significa que hemos llegado al final del body chunked, por lo que devolvemos la posición del final de la petición completa (headers + body) en el buffer de recepción del cliente.
		{
			while (true) //Si hay trailers, avanzamos la posición hasta el final de la petición completa (headers + body + trailers) en el buffer de recepción del cliente. 
			{//CONTINUAR AQUI: Hacer las pruebas que dice CHATGPT (pasadas 1 2 y 3) Y LUEGO PENSAR EN SI HAY QUE PARSEAR LOS TRAILERS.
				lineEnd = buffer.find("\r\n", pos); //Buscamos el final de la linea. Puede haber trailers, que son headers adicionales que se envían después del body chunked. 
				if (lineEnd == std::string::npos)
					return std::string::npos;
				if (lineEnd == pos) //Si encontramos una línea vacía, significa que hemos llegado al final de los trailers y por lo tanto al final de la petición completa (headers + body + trailers) en el buffer de recepción del cliente.
					return pos + 2; // +2 para incluir el \r\n final de la línea vacía.
				pos = lineEnd + 2; //Si la linea no es vacía, avanzamos la posición al final de la línea de trailer +2 para incluir el \r\n y seguimos buscando el final de la petición.
			}
		}
		if (maxBodySize != 0 && (chunkSize > maxBodySize || totalBodySize > maxBodySize - chunkSize))//Si el tamaño actual del body es mayor que el client_max_body_size del config, lanzamos una excepción indicando que el bodyu es demasiado grande. En caso de que la configuracion  tenga client_max_body_size = 0, significa que no hay limite de tamaño de body, por lo que no lanzamos la excepción. (Asi funciona Nginx).
			throw HTTPException(CONTENT_TOO_LARGE, true);
		if (chunkSize > maxValue - totalBodySize) // Comprobamos que la suma no sobrepase el valor máximo de size_t para evitar overflow. Si lo sobrepasa, lanzamos una excepción indicando que la petición es incorrecta.
			throw HTTPException(BAD_REQUEST, true);
		if (chunkSize > buffer.size() - pos) //Si el chunk data no está completo, devolvemos std::string::npos. Lo comprobamos restando para evitar el overflow del size_t en la suma.
			return std::string::npos;
		if (2 > buffer.size() - pos - chunkSize) //Si los 2 bytes del final del chunk no están completos, devolvemos std::string::npos. Lo comprobamos restando para evitar el overflow del size_t en la suma.
			return std::string::npos;
		if (buffer[pos + chunkSize] != '\r' || buffer[pos + chunkSize + 1] != '\n') //Si los 2 bytes del final del chunk no son \r\n, significa que la petición es incorrecta, por lo que lanzamos una excepción.
			throw HTTPException(BAD_REQUEST, true);
		totalBodySize += chunkSize; //Acumulamos el tamaño del body cuando ya tenemos los datos del chunk recibidos correctamente. Para comprobar que no sobrepase el client_max_body_size del config.
		pos += chunkSize + 2; //Avanzamos la posición al final del chunk data más los 2 bytes del \r\n del final del chunk.
	}
}

std::string HTTPRequestParser::cleanChunkedBody(const std::string& rawBody)
{
	std::string cleanedBody;
	size_t pos = 0;
	while (true)
	{
		size_t lineEnd = rawBody.find("\r\n", pos);
		if (lineEnd == std::string::npos)
			throw HTTPException(BAD_REQUEST, true);
		std::string chunkSizeStr = rawBody.substr(pos, lineEnd - pos);
		size_t chunkSize = 0;
		if (!StringUtils::parseSizeT(chunkSizeStr, chunkSize, 16)) //Parseamos el tamaño del chunk en hexadecimal. Si no se puede parsear, lanzamos una excepción
			throw HTTPException(BAD_REQUEST, true);
		if (chunkSize == 0) // Si el tamaño del chunk es 0, significa que hemos llegado al final del body chunked.
			break;
		pos = lineEnd + 2;
		if (chunkSize > rawBody.size() - pos) //Si el chunk data no está completo, lanzamos una excepción indicando que la petición es incorrecta.
			throw HTTPException(BAD_REQUEST, true);
		cleanedBody.append(rawBody.substr(pos, chunkSize)); //Añadimos el chunk data al cleanedBody.
		pos += chunkSize + 2; //Avanzamos la posición al final del chunk data más los 2 bytes del \r\n del final del chunk.
	}
	return cleanedBody;
}

/**
 * Parses the method of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * method: The method string to parse.
 */
void HTTPRequestParser::parseMethod(HTTPRequest &request, const std::string &method)
{
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw HTTPException(NOT_IMPLEMENTED, true);
	request.setMethod(stringToHTTPMethod(method));
}

/**
 * Parses the path of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * path: The path string to parse.
 */
void HTTPRequestParser::parsePath(HTTPRequest &request, const std::string &path)
{
	if (path.empty() || path[0] != '/')
		throw HTTPException(BAD_REQUEST, true);
	request.setPath(path);
}

/**
 * Parses the version of the HTTP request.
 * 
 * request: The HTTPRequest object to populate.
 * version: The version string to parse.
 */
void HTTPRequestParser::parseVersion(HTTPRequest &request, const std::string &version)
{
	//Pensar si aceptamos también HTTP/1.0, o solo HTTP/1.1.
	if (version != "HTTP/1.1" && version != "HTTP/1.0")
		throw HTTPException(BAD_REQUEST, true);
	request.setVersion(version);
}

/**
 * Parses a single header line.
 *
 * headerLine: The header line to parse.
 * key: The key of the header.
 * value: The value of the header.
 */
void HTTPRequestParser::parseHeaderLine(std::string &headerLine, std::string &key, std::string &value)
{
	size_t colonPos = headerLine.find(':');
		if (colonPos == std::string::npos)
			throw HTTPException(BAD_REQUEST, true);

		key = headerLine.substr(0, colonPos);
		if (key.empty()) //IMP: quizás validar que las nombres de los header sean validos o al menos que no tengan espacios dentro? No lo se.
			throw HTTPException(BAD_REQUEST, true);
		value = headerLine.substr(colonPos + 1);

		// Quitamos espacios y tabs del principio del valor
		size_t start = value.find_first_not_of(" \t");
		if (start != std::string::npos) // Si hay al menos un caracter que no sea espacio o tab, eliminamos los espacios y tabs del principio.
			value.erase(0, start);
		else
			value.clear(); // Si el valor es solo espacios, lo dejamos vacío.
}