/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 14:41:53 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/09 15:18:34 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <algorithm>

namespace StringUtils
{
	char toLowerChar(char c);
	std::string toLowerString(const std::string &str);
}

#endif