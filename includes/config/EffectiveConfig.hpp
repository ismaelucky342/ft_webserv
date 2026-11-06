/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EffectiveConfig.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 14:22:06 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/06 16:02:56 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EFFECTIVECONFIG_HPP
#define EFFECTIVECONFIG_HPP

#include <string>
#include "http/HTTPStatus.hpp"

struct EffectiveConfig
{
	std::string root;
	std::string index;
	bool autoindex;
	bool hasRedirect;
	HTTPStatus redirectCode;
	std::string redirectTarget;
};

#endif // EFFECTIVECONFIG_HPP