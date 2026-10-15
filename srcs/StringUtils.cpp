/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 14:46:23 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/15 16:54:22 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StringUtils.hpp"
#include <cctype> // for std::tolower
#include <limits>

/**
 * Converts a character to lowercase.
 * 
 * c: The character to convert.
 * 
 * Returns the lowercase version of the character.
 */
char StringUtils::toLowerChar(char c)
{
	return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

/**
 * Converts a string to lowercase.
 * 
 * str: The string to convert.
 * 
 * Returns the lowercase version of the string.
 */
std::string StringUtils::toLowerString(const std::string &str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), StringUtils::toLowerChar);
	return lowerStr;
}

/**
 * Parses a string to a size_t value.
 * 
 * str: The string to parse.
 * result: Reference to store the parsed size_t value.
 * 
 * Returns true if parsing was successful, false otherwise.
 */
bool StringUtils::parseSizeT(const std::string &str, size_t &result)
{
	if (str.empty())
		return false;

	size_t value = 0;
	size_t maxValue = std::numeric_limits<size_t>::max();

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(str[i])))
			return false;

		size_t digit = str[i] - '0';

		if (value > (maxValue - digit) / 10) // value × 10 + digit <= maxValue ---> despejando value:  value > (maxValue - digit) / 10
			return false;

		value = value * 10 + digit;
	}

	result = value;
	return true;
}

/**
 * Parses a string to a long value.
 * 
 * str: The string to parse.
 * 
 * Returns the parsed long value.
 * Throws std::runtime_error if the string is not a valid number.
 */
long StringUtils::parseLong(const std::string &str)
{
	char *end;
	long value = std::strtol(
		str.c_str(), &end,
		10); // Convertimos el token actual a un número entero (long) usando strtol. end apunta al primer caracter que no se pudo convertir. (el 10 es la base a lo que lo queremos convertir)
	if (*end !=
		'\0') // Si end no apunta al final de la cadena, significa que hubo caracteres no numéricos en el token actual, por lo que lanzamos un error.
		throw std::runtime_error("Invalid number: " + str);
	return value;
}