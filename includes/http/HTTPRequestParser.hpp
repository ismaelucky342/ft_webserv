#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include "http/HTTPRequest.hpp"
#include <vector>

class HTTPRequestParser
{
public:
	HTTPRequestParser();
	HTTPRequestParser(const HTTPRequestParser &other);
	HTTPRequestParser &operator=(const HTTPRequestParser &other);
	~HTTPRequestParser();

	HTTPRequest parse(const std::string &rawRequest);
	void parseRequestLine(HTTPRequest &request, const std::string &rawRequest);
	void parseHeaders(HTTPRequest& request, const std::string& rawRequest);
	bool parseContentLengthBody(const std::string &buffer, size_t bodyStart, size_t bodyLength, size_t &requestEnd, std::string &body);
	bool parseChunkedBody(const std::string &buffer, size_t bodyStart, const size_t maxBodySize, size_t &requestEnd, std::string &body);
	void parseNormalizedBody(HTTPRequest& request, const std::string& rawBody);
	size_t getHeadersEnd(const std::string &buffer);
	
	private:
	void parseMethod(HTTPRequest &request, const std::string &method);
	void parsePath(HTTPRequest &request, const std::string &path);
	void parseVersion(HTTPRequest &request, const std::string &version);
	void parseHeaderLine(std::string &headerLine, std::string &key, std::string &value);
	size_t getContentLengthRequestEnd(const std::string &buffer, size_t bodyStart, size_t bodyLength);
	size_t getChunkedRequestEnd(const std::string &buffer, size_t bodyStart, const size_t maxBodySize);
	std::string cleanChunkedBody(const std::string& rawBody);
};

#endif