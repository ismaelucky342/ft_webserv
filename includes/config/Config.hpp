/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 15:25:37 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/08/04 11:19:44 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "colors.hpp"
#include "config/Listen.hpp"
#include "config/Location.hpp"

class Config
{
public:
	Config();
	Config(const Config &other);
	Config &operator=(const Config &other);
	~Config();

	const std::string&				getServerName() const;
	const std::vector<Listen>&		getListens() const;
	std::string						getRoot() const;
	std::string						getIndex() const;
	long							getClientMaxBodySize() const;
	std::string						getErrorPage(int errorCode) const;
	const std::vector<Location>&	getLocations() const;

	void setServerName(const std::string &serverName);
	void addListen(const Listen& listen);
	void setRoot(const std::string &root);
	void setIndex(const std::string &index);
	void setClientMaxBodySize(long clientMaxBodySize);
	void addErrorPage(int errorCode, const std::string &errorPagePath);
	void addLocation(const Location& location);

	void print() const;

private:
	std::string 				_serverName;
	std::vector<Listen>			_listens;
	std::string					_root;
	std::string 				_index;
	long						_clientMaxBodySize;
	std::map<int, std::string>	_errorPages; // Map para almacenar las páginas de error personalizadas, donde la clave es el código de error y el valor es la ruta del archivo HTML correspondiente.
	std::vector<Location>		_locations;
};

#endif