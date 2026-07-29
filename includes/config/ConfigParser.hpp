/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 14:38:09 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/29 16:41:59 by mvidal-h         ###   ########.fr       */
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
	void parseRoot(Config &config);
	void parseIndex(Config &config);
	void parseErrorPage(Config &config);
	void parseOther(); // BORRAR Auxiliar para imprimir los bloques que aun no parseo. Luego se eliminará

	long strToLong(const std::string &str);
};

#endif