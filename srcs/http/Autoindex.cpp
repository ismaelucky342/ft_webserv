/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 11:40:13 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/27 18:44:05 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::cout
#include <sstream> // std::ostringstream
#include <dirent.h> // opendir(), readdir(), closedir() Para leer el contenido de un directorio.
#include <sys/stat.h> // stat() Para obtener info de un archivo (tipo, tamaño, permisos, etc.)
#include <vector>
#include <ctime>
#include <algorithm> // std::sort

#include "http/Autoindex.hpp" 
#include "http/HTTPException.hpp"
#include "http/HTTPStatus.hpp"
#include "colors.hpp"

/**
 * Generates an autoindex body for the specified directory path.
 * @param srcPath The directory path for which to generate the autoindex body.
 * @param requestPath The path of the request.
 * @return The generated autoindex body as a string.
 * @throws HTTPException If the directory cannot be opened (e.g., due to permissions).
 */
std::string AutoIndex::generateBody(const std::string &srcPath, const std::string &requestPath)
{
	DIR* dir = opendir(srcPath.c_str());
	if (dir == NULL) {
		throw HTTPException(FORBIDDEN, false); // (IMP: revisar el error a deolver.) No se puede abrir el directorio, devolvemos un error 403 Forbidden.
	}
	std::cout << YELLOW << "Opened directory: " << srcPath << RESET << std::endl; //IMP: PARA DEPURACION BORRAR LUEGO.
	
	std::vector<AutoIndex::DirectoryEntry> entries;
	struct dirent* ent;
	while ((ent = readdir(dir)) != NULL)
	{
		std::string name = ent->d_name;
		if (name == "." || (name == ".." && requestPath == "/")) // Skip current directory entry and parent directory entry when at the root request path
			continue; // Skip current directory entry
		std::string fullPath = srcPath;
		if (fullPath[fullPath.length() - 1] != '/')
			fullPath += "/";
		fullPath += name;
		struct stat fileInfo;
		if (stat(fullPath.c_str(), &fileInfo) != 0)
			continue;

		AutoIndex::DirectoryEntry entry;
		entry.name = name;
		entry.isDirectory = S_ISDIR(fileInfo.st_mode);
		entry.tam = fileInfo.st_size;
		entry.fecha = fileInfo.st_mtime;
		entries.push_back(entry);
	}
	closedir(dir);
	std::sort(entries.begin(), entries.end(), AutoIndex::compareEntries);

	std::string displayPath = escapeHtml(requestPath);
	std::ostringstream bodystream;
	bodystream << "<!DOCTYPE html>\n<html lang=\"es\">\n<head>\n"
			  << "<meta charset=\"UTF-8\">\n"
			  << "<title>Indice de " << displayPath << "</title>\n"
			  << "<style>body{font-family:monospace;margin:2rem}"
			  << "td,th{padding:4px 16px;text-align:left}</style>\n"
			  << "</head>\n<body>\n"
			  << "<h1>Indice de " << displayPath << "</h1>\n"
			  << "<table>\n<tr><th>Nombre</th><th>Tamaño</th><th>Modificado</th></tr>\n";
	for (std::vector<AutoIndex::DirectoryEntry>::const_iterator it = entries.begin(); it != entries.end(); ++it)
	{
		std::string encodedName = encodeUrl(it->name);
		std::string displayName = escapeHtml(it->name);
		if (it->isDirectory)
		{
			encodedName += "/"; // Append a slash to directory names for clarity
			displayName += "/"; // Append a slash to directory names for clarity
		}
		bodystream << "<tr><td><a href=\"" << encodedName << "\">" << displayName << "</a></td>"
				   << "<td>" << it->tam << "</td>"
				   << "<td>" << AutoIndex::formatDate(it->fecha) << "</td></tr>\n"; // localtime devuelve la hora pasada como parametro(un time_t) en un struct tm y asctime convertira esa tm a string. (IMP: revisar el formato de fecha y hora que devuelve asctime, porque devuelve un salto de linea al final y eso puede romper el html.)
	}
	bodystream << "</table>\n"
			   << "</body>\n</html>";
	std::string body = bodystream.str();
	return body;
}

/**
 * Escapes special HTML characters in a string to prevent XSS attacks.
 * XSS means Cross-Site Scripting, which is a type of security vulnerability that
 * allows attackers to inject malicious scripts into web pages viewed by other users.
 * By escaping special HTML characters, we ensure that any potentially harmful code
 * is rendered harmless and displayed as plain text instead of being executed by the browser.
 * @param str The string to escape.
 * @return The escaped string.
 */
std::string AutoIndex::escapeHtml(const std::string &str)
{
	std::string result;
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		switch (str[i])
		{
		case '&':
			result.append("&amp;");
			break;
		case '<':
			result.append("&lt;");
			break;
		case '>':
			result.append("&gt;");
			break;
		case '"':
			result.append("&quot;");
			break;
		case '\'':
			result.append("&#39;");
			break;
		default:
			result.push_back(str[i]);
		}
	}
	return result;
}

/**
 * Encodes a string for use in a URL.
 * This function percent-encodes all characters that are not alphanumeric or '-_.~'.
 * Allows file and directory names to be used as part of a URL without their
 *special characters being interpreted as URL components.
 * @param str The string to encode.
 * @return The encoded string.
 */
std::string AutoIndex::encodeUrl(const std::string &str)
{
	static const char hex[] = "0123456789ABCDEF";
	std::string result;
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		unsigned char c = static_cast<unsigned char>(str[i]);
		if ((c >= '0' && c <= '9')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= 'a' && c <= 'z')
			|| (c == '-' || c == '_' || c == '.' || c == '~'))
		{ // Alphanumeric and some special characters are not encoded
			result.push_back(c);
		} else { // All other characters are percent-encoded. It means that they are replaced with a '%' followed by their ASCII value in hexadecimal format. This ensures that the URL is valid and safe for use in hyperlinks.
			result.push_back('%');
			result.push_back(hex[(c >> 4) & 0xF]); //c respresenta el caracter a codificar. imaginemos el espacio (32 en ASCII, 00100000 en binario y x20 en hexadecimal). c >> 4 desplaza los bits 4 posiciones a la derecha, resultando en 00000010 (2 en decimal). Luego, (c >> 4) & 0xF realiza una operación AND con 0xF (00001111 en binario), lo que nos da el valor de los 4 bits más significativos del caracter. En este caso, el resultado es 2, que corresponde al primer dígito hexadecimal del espacio leyendo su posición en el array hex.
			result.push_back(hex[c & 0xF]); // c & 0xF realiza una operación AND con 0xF (00001111 en binario) sin desplazar nada, lo que nos da el valor de los 4 bits menos significativos del caracter. En este caso, el resultado es 0, que corresponde al segundo dígito hexadecimal del espacio leyendo su posición en el array hex.
		}
	}
	return result;
}

/**
 * Formats a time_t value into a human-readable date string.
 * This function converts a time_t value into a string in the format "YYYY-MM-DD HH:MM".
 * @param t The time_t value to format.
 * @return The formatted date string.
 */
std::string AutoIndex::formatDate(time_t t)
{
	char buf[32];
	struct tm *timeInfo = std::localtime(&t);

	if (timeInfo == NULL)
		return "";

	if (std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", timeInfo) == 0)
		return "";

	return buf;
}

/**
 * Compares two DirectoryEntry objects for sorting purposes.
 * Directories are prioritized over files, and entries are sorted alphabetically by name.
 * @param a The first DirectoryEntry to compare.
 * @param b The second DirectoryEntry to compare.
 * @return true if the first entry should come before the second, false otherwise.
 */
bool AutoIndex::compareEntries(const DirectoryEntry &a, const DirectoryEntry &b)
{
	if (a.isDirectory != b.isDirectory) // Si uno es
		return a.isDirectory; // Directories come before files
	return a.name < b.name; // Sort alphabetically by name
}