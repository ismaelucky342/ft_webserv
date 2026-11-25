/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 11:40:13 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/25 11:47:00 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::cout
#include <sstream> // std::ostringstream
#include <dirent.h> // opendir(), readdir(), closedir() Para leer el contenido de un directorio.
#include "http/Autoindex.hpp" 
#include "http/HTTPException.hpp"
#include "http/HTTPStatus.hpp"
#include "colors.hpp"

std::string AutoIndex::generate(const std::string &path)
{
	std::ostringstream bodystream;
	bodystream << "<html><head>"
			   << "<title>Index of " << path << "</title>"
			   << "</head><body>"
			   << "<h1>Index of " << path << "</h1>"
			   << "<hr><ul>";
	DIR* dir = opendir(path.c_str());
	if (dir == NULL) {
		throw HTTPException(FORBIDDEN, false); // (IMP: revisar el error a deolver.) No se puede abrir el directorio, devolvemos un error 403 Forbidden.
	}
	std::cout << YELLOW << "Opened directory: " << path << RESET << std::endl; //IMP: PARA DEPURACION BORRAR LUEGO.
	struct dirent* ent;
	while ((ent = readdir(dir)) != NULL) {
		std::string name = ent->d_name;
		if (name == "." || name == "..") {
			continue; // Skip current and parent directory entries
		}
		bodystream << "<li><a href=\"" << name << "\">" << name << "</a></li>";
	}
	closedir(dir);
	bodystream << "</ul><hr>"
			   << "</body></html>";
	std::string body = bodystream.str();
	return body;
}