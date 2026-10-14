/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 14:41:53 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/14 15:42:19 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace StringUtils
{
	char toLowerChar(char c);
	std::string toLowerString(const std::string &str);

	bool parseSizeT(const std::string &str, size_t &result);
	long parseLong(const std::string &str);
}

#endif