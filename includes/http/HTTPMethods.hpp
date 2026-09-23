/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPMethods.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 11:35:24 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/09/23 11:28:37 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHODS_HPP
#define HTTPMETHODS_HPP

#include <string>
#include <stdexcept>

enum HTTPMethod
{
	GET,
	POST,
	DELETE
};

HTTPMethod stringToHTTPMethod(const std::string &method);

#endif