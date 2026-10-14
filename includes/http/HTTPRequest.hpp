#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include "http/HTTPMethods.hpp"

class HTTPRequest
{
public:
	HTTPRequest();
	HTTPRequest(const HTTPRequest &other);
	HTTPRequest &operator=(const HTTPRequest &other);
	~HTTPRequest();
	
	bool isRequestLineParsed() const;
	HTTPMethod getMethod() const;
	const std::string &getPath() const;
	const std::string &getQueryString() const;
	const std::string &getVersion() const;
	bool areHeadersParsed() const;
	const std::map<std::string, std::string> &getHeaders() const;
	bool hasHeader(const std::string &key) const; // Devuelve true si el header existe, false si no existe.
	const std::string getHeader(const std::string &key) const; // Se devuelve una copia del valor del header, ya que si devolvieramos una referencia a un string que no existe en el map, se produciría un error de acceso a memoria.
	bool isBodyParsed() const;
	const std::string &getBody() const;

	void setRequestLineParsed(bool parsed);
	void setMethod(HTTPMethod method);
	void setPath(const std::string &path);
	void setQueryString(const std::string &queryString);
	void setVersion(const std::string &version);
	void setHeadersParsed(bool parsed);
	void setHeader(const std::string &key, const std::string &value);
	void setBodyParsed(bool parsed);
	void setBody(const std::string &newBody);

	void print() const; // Method to print the HTTPRequest object for debugging purposes
	void reset();

private:
	bool _requestLineParsed;
	HTTPMethod _method;
	std::string _path;
	std::string _queryString;
	std::string _version;
	bool _headersParsed;
	std::map<std::string, std::string> _headers;
	bool _bodyParsed;
	std::string _body;
};

#endif