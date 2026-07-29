/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 11:21:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/07/29 12:03:22 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include "http/HTTPMethods.hpp"
#include <set>

class Location
{
public:
	Location(const std::string &path);
	Location(const Location &other);
	Location &operator=(const Location &other);
	~Location();

	const std::string &getPath() const;

	bool hasRoot() const;
	const std::string &getRoot() const;
	bool hasIndex() const;
	const std::string &getIndex() const;
	bool hasAutoindex() const;
	bool getAutoindex() const;
	const std::set<HTTPMethod> &getAllowedMethods() const;
	bool hasRedirect() const;
	int getRedirectCode() const;
	const std::string &getRedirectTarget() const;
	bool hasUploadStore() const;
	const std::string &getUploadStore() const;
	bool hasCgi() const;
	const std::string &getCgiExtension() const;
	const std::string &getCgiExecutable() const;

	void setRoot(const std::string& root);
	void setIndex(const std::string& index);
	void setAutoindex(bool enabled);
	void addAllowedMethod(HTTPMethod method);
	void setRedirect(int code, const std::string& target);
	void setUploadStore(const std::string& path);
	void setCgi(const std::string& extension, const std::string& executable);

private:
	std::string _path;

	bool _hasRoot;
	std::string _root;

	bool _hasIndex;
	std::string _index;

	bool _hasAutoindex;
	bool _autoindex;

	std::set<HTTPMethod> _allowedMethods;

	bool _hasRedirect;
	int _redirectCode;
	std::string _redirectTarget;

	bool _hasUploadStore;
	std::string _uploadStore;

	bool _hasCgi;
	std::string _cgiExtension;
	std::string _cgiExecutable;
};

#endif