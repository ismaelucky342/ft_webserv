/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 11:10:52 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/25 18:32:31 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class AutoIndex
{
public:
	static std::string generateBody(const std::string &srcPath, const std::string &requestPath);

private:
	struct DirectoryEntry
	{
		std::string name;
		bool isDirectory;
		off_t tam;
		time_t fecha;
	};

	static std::string escapeHtml(const std::string &str); // Escapes special HTML characters in a string to prevent XSS attacks.(Nombre visible en HTML)
	static std::string encodeUrl(const std::string &str); // Encodes special characters in a URL to ensure it is valid and safe for use in hyperlinks.(Nombre dentro de href)
};