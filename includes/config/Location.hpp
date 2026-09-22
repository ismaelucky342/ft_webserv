/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 11:21:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/09/22 17:36:50 by mvidal-h         ###   ########.fr       */
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
	bool hasAllowedMethods() const;
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
	void setHasRoot(bool hasRoot);
	void setIndex(const std::string& index);
	void setHasIndex(bool hasIndex);
	void setAutoindex(bool enabled);
	void setHasAutoindex(bool hasAutoindex);
	void addAllowedMethod(HTTPMethod method);
	void setHasAllowedMethods(bool hasAllowedMethods);
	void setRedirect(int code, const std::string& target);
	void setHasRedirect(bool hasRedirect);
	void setUploadStore(const std::string& path);
	void setHasUploadStore(bool hasUploadStore);
	void setCgi(const std::string& extension, const std::string& executable);
	void setHasCgi(bool hasCgi);

private:
	std::string _path; //la ruta del url

	bool _hasRoot;
	std::string _root; // La ruta del equipo

	bool _hasIndex; //Indica que hacer cuando la ruta es un directorio
	std::string _index;

	bool _hasAutoindex; //Indica si se puede hacer un listado de los ficheros del directorio en caso de que no haya un _index
	bool _autoindex;

	bool _hasAllowedMethods; // NUEVO: Indica si se han definido métodos permitidos para esta location
	std::set<HTTPMethod> _allowedMethods; //los métodos permitidos para esa location.

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