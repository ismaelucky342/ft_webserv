/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 11:10:52 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/25 11:27:07 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class AutoIndex
{
public:
	static std::string generate(const std::string &path);

private:
	struct DirectoryEntry
	{
		std::string name;
		bool isDirectory;
	};

	static std::string escapeHtml(const std::string &str);
	static std::string encodeUrl(const std::string &str);
};