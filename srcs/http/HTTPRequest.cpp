/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:54:36 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/14 17:52:07 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HTTPRequest.hpp"
#include <iostream>
#include "colors.hpp"
#include "StringUtils.hpp"

/**
 * Default constructor for the HTTPRequest class.
 */
HTTPRequest::HTTPRequest() :_requestLineParsed(false), _method(METHOD_NONE), _path(""), _queryString(""), _version(""), _headersParsed(false), _bodyParsed(false)
{
	std::cout << BOLD_GREEN << "HTTPRequest default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the HTTPRequest class.
 * 
 * other: The HTTPRequest object to copy.
 */
HTTPRequest::HTTPRequest(const HTTPRequest &other)
	: _requestLineParsed(other._requestLineParsed), _method(other._method), _path(other._path), _queryString(other._queryString), _version(other._version), _headersParsed(other._headersParsed),
	  _headers(other._headers), _bodyParsed(other._bodyParsed), _body(other._body)
{
	std::cout << BOLD_GREEN << "HTTPRequest copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the HTTPRequest class.
 * 
 * other: The HTTPRequest object to assign.
 * @return A reference to the assigned object.
 */
HTTPRequest &HTTPRequest::operator=(const HTTPRequest &other)
{
	if (this != &other)
	{
		_requestLineParsed = other._requestLineParsed;
		_method = other._method;
		_path = other._path;
		_queryString = other._queryString;
		_version = other._version;
		_headersParsed = other._headersParsed;
		_headers = other._headers;
		_bodyParsed = other._bodyParsed;
		_body = other._body;
	}
	std::cout << BOLD_GREEN << "HTTPRequest assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the HTTPRequest class.
 */
HTTPRequest::~HTTPRequest()
{
	std::cout << BOLD_RED << "HTTPRequest destructor called" << RESET << std::endl;
}

/**
 * Returns true if the request line has been parsed, false otherwise.
 * 
 * @return A boolean value indicating if the request line is parsed.
 */
bool HTTPRequest::isRequestLineParsed() const
{
	return _requestLineParsed;
}

/**
 * Returns the HTTP method of the request.
 * 
 * @return The HTTP method value.
 */
HTTPMethod HTTPRequest::getMethod() const
{
	return _method;
}

/**
 * Returns the path of the request.
 * 
 * @return A reference to the path string.
 */
const std::string &HTTPRequest::getPath() const
{
	return _path;
}

/**
 * Returns the HTTP version of the request.
 * 
 * @return A reference to the HTTP version string.
 */
const std::string &HTTPRequest::getVersion() const
{
	return _version;
}

/**
 * Returns true if the headers have been parsed, false otherwise.
 * 
 * @return A boolean value indicating if the headers are parsed.
 */
bool HTTPRequest::areHeadersParsed() const
{
	return _headersParsed;
}

/**
 * Returns the headers of the request.
 * 
 * @return A reference to the headers map.
 */
const std::map<std::string, std::string> &HTTPRequest::getHeaders() const
{
	return _headers;
}

/**
 * Checks if a specific header exists in the request.
 * 
 * @param key The header key to look for.
 * @return true if the header exists, false otherwise.
 */
bool HTTPRequest::hasHeader(const std::string &key) const
{
	return _headers.find(StringUtils::toLowerString(key)) != _headers.end();
}

/**
 * Returns a specific header from the request.
 * 
 * @param key The header key to look for.
 * @return A reference to the header value string.
 */
const std::string HTTPRequest::getHeader(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find(StringUtils::toLowerString(key));
	if (it != _headers.end())
		return it->second;
	return std::string(); // Return an empty string if the header is not found
}

/**
 * Returns true if the body has been parsed, false otherwise.
 * 
 * @return A boolean value indicating if the body is parsed.
 */
bool HTTPRequest::isBodyParsed() const
{
	return _bodyParsed;
}

/**
 * Returns the body of the request.
 * 
 * @return A reference to the body string.
 */
const std::string &HTTPRequest::getBody() const
{
	return _body;
}

/**
 * Sets the request line parsed status.
 *
 * parsed: The boolean value indicating if the request line is parsed.
 */
void HTTPRequest::setRequestLineParsed(bool parsed)
{
	_requestLineParsed = parsed;
}

/**
 * Sets the headers parsed status.
 *
 * parsed: The boolean value indicating if the headers are parsed.
 */
void HTTPRequest::setHeadersParsed(bool parsed)
{
	_headersParsed = parsed;
}

/**
 * Sets the body parsed status.
 *
 * parsed: The boolean value indicating if the body is parsed.
 */
void HTTPRequest::setBodyParsed(bool parsed)
{
	_bodyParsed = parsed;
}

/**
 * Sets the HTTP method of the request.
 * 
 * method: The HTTP method to set.
 */
void HTTPRequest::setMethod(HTTPMethod method)
{
	_method = method;
}

/**
 * Sets the path of the request.
 *
 * path: The path string to set.
 */
void HTTPRequest::setPath(const std::string &path)
{
	_path = path;
}

/**
 * Sets the HTTP version of the request.
 *
 * version: The HTTP version string to set.
 */
void HTTPRequest::setVersion(const std::string &version)
{
	_version = version;
}

/**
 * Sets a header for the request.
 *
 * key: The header key.
 * value: The header value.
 */
void HTTPRequest::setHeader(const std::string &key, const std::string &value)
{
	_headers[key] = value;
}

/**
 * Sets the body of the request.
 *
 * newBody: The new body string.
 */
void HTTPRequest::setBody(const std::string &newBody)
{
	_body = newBody;
}

/**
 * Prints the HTTPRequest object for debugging purposes.
 */
void HTTPRequest::print() const
{
	std::cout << BOLD_GREEN << "HTTPRequest values:" << RESET << std::endl;
	std::cout << "Method: " << _method << std::endl;
	std::cout << "Path: " << _path << std::endl;
	// std::cout << "Query String: " << _queryString << std::endl;
	std::cout << "Version: " << _version << std::endl;
	std::cout << "Headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	std::cout << "Body: " << _body << std::endl;
}

/**
 * Resets the HTTPRequest object to its default state.
 */
void HTTPRequest::reset()
{
	_requestLineParsed = false;
	_method = METHOD_NONE;
	_path.clear();
	_queryString.clear();
	_version.clear();
	_headersParsed = false;
	_headers.clear();
	_bodyParsed = false;
	_body.clear();
}