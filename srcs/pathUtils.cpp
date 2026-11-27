/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 18:55:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/27 17:59:30 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "pathUtils.hpp"

/**
 * @brief Normalizes a path by resolving "." and ".." segments.
 *        Its behavior depends on the provided options, allowing paths to
 *        preserve leading "..", ignore excess parent segments or reject
 *        attempts to traverse beyond the allowed directory.
 * @param path The path whose segments will be normalized.
 * @param segments The vector where the resulting segments are stored.
 * @param allowLeadingParent Whether unresolved ".." segments at the beginning
 *        of a relative path should be preserved instead of rejected.
 * @param ignoreExcessParent Whether unresolved ".." segments should be ignored
 *        instead of rejected when they cannot be resolved.
 * @return true if the path is successfully normalized, false otherwise.
 */
static bool normalizeSegments(const std::string &path, std::vector<std::string> &segments, bool allowLeadingParent, bool ignoreExcessParent)
{
	std::string::size_type start = 0;

	while (start < path.length())
	{
		std::string::size_type end = path.find('/', start);

		if (end == std::string::npos)
			end = path.length();

		std::string segment = path.substr(start, end - start);

		if (segment == "..")
		{
			if (segments.empty() || segments.back() == "..")
			{
				if (allowLeadingParent)
					segments.push_back(segment);
				else if (!ignoreExcessParent)
					return false;
			}
			else
				segments.pop_back();
		}
		else if (!segment.empty() && segment != ".")
			segments.push_back(segment);

		if (end == path.length())
			break;

		start = end + 1;
	}

	return true;
}

/**
 * @brief Normalizes a filesystem path by resolving "." and ".." segments.
 *        Preserves leading ".." segments in relative paths and prevents
 *        absolute paths from traversing above the filesystem root.
 * @param path The filesystem path to normalize.
 * @param normalizedPath The resulting normalized filesystem path.
 * @return true if the path is successfully normalized, false otherwise.
 */
bool pathUtils::normalizeFileSystemPath(const std::string &root, std::string &normalizedRoot)
{
	if (root.empty())
		return false;
	bool isAbsolute = (root[0] == '/');
	std::string relativeRoot = root;

	if (isAbsolute)
		relativeRoot.erase(0, 1);

	std::vector<std::string> segments;
	if (!normalizeSegments(relativeRoot, segments, !isAbsolute, isAbsolute))
		return false;

	if (isAbsolute)
		normalizedRoot = "/";
	else
		normalizedRoot = "";
	for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
	{
		if (!normalizedRoot.empty() && normalizedRoot[normalizedRoot.length() - 1] != '/')
			normalizedRoot += "/";
		normalizedRoot += *it;
	}
	if (!isAbsolute && normalizedRoot.empty())
		normalizedRoot = ".";

	return true;
}

/**
 * @brief Resolves a client request path relative to the configured root.
 *        Rejects attempts to traverse outside the root directory using
 *        ".." segments.
 * @param root The normalized filesystem root directory.
 * @param requestPath The URL path requested by the client.
 * @param resolvedPath The resulting normalized filesystem path.
 * @return true if the path is valid and remains within the configured root,
 *         false otherwise.
 */
bool pathUtils::normalizePath(const std::string &root, const std::string &requestPath, std::string &resolvedPath)
{
	std::vector<std::string> segments; //donde almacenaremos los segmentos de la ruta que no sean "." o ".."
	std::string relativePath = requestPath;

	if (!relativePath.empty() && relativePath[0] == '/')
		relativePath.erase(0, 1);
	if (!normalizeSegments(relativePath, segments, false, false))
		return false;

	resolvedPath = root;
	for (std::vector<std::string>::const_iterator it = segments.begin(); it != segments.end(); ++it)
	{
		if (resolvedPath[resolvedPath.length() - 1] != '/')
			resolvedPath += "/";
		resolvedPath += *it;
	}
	return true;
}
