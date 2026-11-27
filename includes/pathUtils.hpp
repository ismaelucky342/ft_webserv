/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 18:54:36 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/27 17:56:45 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATHUTILS_HPP
#define PATHUTILS_HPP

#include <string>

namespace pathUtils
{
	bool normalizeFileSystemPath(
		const std::string &root,
		std::string &normalizedRoot
	);

	bool normalizePath(
		const std::string &root,
		const std::string &requestPath,
		std::string &resolvedPath
	);
}

#endif