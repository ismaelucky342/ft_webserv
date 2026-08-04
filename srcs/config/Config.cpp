/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:28:35 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/08/04 11:19:26 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Config.hpp"
#include <fstream>
#include <sstream>
#include <vector>

/**
 * Default constructor for the Config class. Initializes default values for
 * port, root, and index.
 * 
 * port: Default port number (8080).
 * root: Default root directory ("./").
 * index: Default index file ("index.html").
 * host: Default host ("0.0.0.0").
 */
Config::Config() : _serverName(""), _root("./"), _index("index.html"), _clientMaxBodySize(1024L * 1024L) // 1 MB default
{
	std::cout << BOLD_GREEN << "Config default constructor called" << RESET << std::endl;
}

/**
 * Copy constructor for the Config class. Creates a new Config object as a copy
 * of another.
 */
Config::Config(const Config &other)
	: _serverName(other._serverName), _listens(other._listens), _root(other._root), _index(other._index), _clientMaxBodySize(other._clientMaxBodySize), _errorPages(other._errorPages), _locations(other._locations)
{
	std::cout << BOLD_GREEN << "Config copy constructor called" << RESET << std::endl;
}

/**
 * Assignment operator for the Config class. Assigns values from another Config
 * object to this one.
 */
Config &Config::operator=(const Config &other)
{
	if (this != &other)
	{
		_serverName = other._serverName;
		_listens = other._listens;
		_root = other._root;
		_index = other._index;
		_clientMaxBodySize = other._clientMaxBodySize;
		_errorPages = other._errorPages;
		_locations = other._locations;
	}
	std::cout << BOLD_GREEN << "Config assignment operator called" << RESET << std::endl;
	return *this;
}

/**
 * Destructor for the Config class. Cleans up resources if necessary.
 */
Config::~Config()
{
	std::cout << BOLD_RED << "Config destructor called" << RESET << std::endl;
}

/**
 * Gets the server name.
 * @return A constant reference to the server name.
 */
const std::string& Config::getServerName() const
{
	return _serverName;
}

/**
 * Gets the list of Listen objects.
 * @return A constant reference to the vector of Listen objects.
 */
const std::vector<Listen>& Config::getListens() const
{
	return _listens;
}

/**
 * Gets the root directory.
 * @return The root directory.
 */
std::string Config::getRoot() const
{
	return _root;
}

/**
 * Gets the index file.
 * @return The index file.
 */
std::string Config::getIndex() const
{
	return _index;
}

/**
 * Gets the maximum allowed size for client request bodies.
 * @return The maximum allowed size in bytes.
 */
long Config::getClientMaxBodySize() const
{
	return _clientMaxBodySize;
}

/*
 * Gets the error page path for a specific error code.
 * @param errorCode The HTTP error code.
 * @return The path to the corresponding error page, or an empty string if not set.
 */
std::string Config::getErrorPage(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = _errorPages.find(errorCode);
	if (it != _errorPages.end())
		return it->second;
	return "";
}

/**
 * Gets the list of Location objects.
 * @return A constant reference to the vector of Location objects.
 */
const std::vector<Location>& Config::getLocations() const
{
	return _locations;
}

/**
 * Sets the server name.
 * @param serverName The server name.
 */
void Config::setServerName(const std::string &serverName)
{
	_serverName = serverName;
}

/**
 * Adds a Listen object to the configuration.
 * @param listen The Listen object to add.
 */
void Config::addListen(const Listen& listen)
{
	_listens.push_back(listen);
}

/**
 * Sets the root directory.
 * @param root The root directory.
 */
void Config::setRoot(const std::string &root)
{
	_root = root;
}

/**
 * Sets the index file.
 * @param index The index file.
 */
void Config::setIndex(const std::string &index)
{
	_index = index;
}

/**
 * Sets the maximum allowed size for client request bodies.
 * @param size The maximum allowed size in bytes.
 */
void Config::setClientMaxBodySize(long clientMaxBodySize)
{
	_clientMaxBodySize = clientMaxBodySize;
}

/**
 * Sets the error page path for a specific error code.
 * @param errorCode The HTTP error code.
 * @param errorPagePath The path to the corresponding error page.
 */
void Config::addErrorPage(int errorCode, const std::string &errorPagePath)
{
	_errorPages.insert(std::make_pair(errorCode, errorPagePath));
}

/**
 * Adds a Location object to the configuration.
 * @param location The Location object to add.
 */
void Config::addLocation(const Location& location)
{
	_locations.push_back(location);
}

/**
 * Prints the configuration values.
 */
void Config::print() const
{
	std::cout << BOLD_GREEN << "Config values:" << RESET << std::endl;
	std::cout << "  Server Name: " << _serverName << std::endl;
	std::cout << "  Listens:" << std::endl;
	for (size_t i = 0; i < _listens.size(); ++i)
		std::cout << "    Port: " << _listens[i].getPort() << ", Interface: " << _listens[i].getInterface() << std::endl;
	std::cout << "  Root: " << _root << std::endl;
	std::cout << "  Index: " << _index << std::endl;
	std::cout << "  Client Max Body Size: " << _clientMaxBodySize << std::endl;
	std::cout << "  Error Pages:" << std::endl;
	for (std::map<int, std::string>::const_iterator it = _errorPages.begin();
		 it != _errorPages.end(); ++it)
		std::cout << "    " << it->first << ": " << it->second << std::endl;
	std::cout << "  Locations:" << std::endl;
	for (size_t i = 0; i < _locations.size(); ++i)
		std::cout << "    Path: " << _locations[i].getPath() << std::endl;
}