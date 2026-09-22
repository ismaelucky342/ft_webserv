/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 14:38:09 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/09/22 17:54:16 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "config/Config.hpp"
#include <vector>

class ConfigParser
{
public:
	ConfigParser();
	ConfigParser(const ConfigParser &other);
	ConfigParser &operator=(const ConfigParser &other);
	~ConfigParser();

	std::vector<Config> parse(const std::string &filename);

private:
	std::vector<std::string> _tokens;
	size_t _pos;

	const std::string &current() const;
	void next();
	void expect(const std::string &token);

	Config parseServer();
	void parseListen(Config &config);
	void parseServerName(Config &config);
	const std::string &parseRoot();
	const std::string &parseIndex();
	void parseErrorPage(Config &config);
	void parseOther(); // BORRAR Auxiliar para imprimir los bloques que aun no parseo. Luego se eliminará
	void parseLocation(Config &config); //Los de location para pruebas mientras unificamos.
	void parseAutoindex(Location &location);
	void parseAllowMethods(Location &location);
	void parseRedirect(Location &location);
	void parseUploadStore(Location &location);
	void parseCgi(Location &location);

	long strToLong(const std::string &str); //IMP: Estas y la funcion de abajo tendremos que mantenerlas
	HTTPMethod stringToHTTPMethod(const std::string &method);
};

#endif