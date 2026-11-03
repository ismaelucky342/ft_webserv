/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethods.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 11:35:24 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/06 11:17:47 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHODS_HPP
#define HTTPMETHODS_HPP

#include <string>
#include <stdexcept>

enum HTTPMethod
{
	METHOD_NONE,
	GET,
	POST,
	DELETE
};

HTTPMethod stringToHTTPMethod(const std::string &method);

#endif