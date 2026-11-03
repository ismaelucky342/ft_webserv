/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 13:06:19 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/27 10:51:20 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

#include <exception>

class HTTPException : public std::exception
{
private:
	int		_statusCode;
	bool	_closeConnection;

public:
	HTTPException(int statusCode, bool closeConnection);
	HTTPException(const HTTPException &other);
	HTTPException &operator=(const HTTPException &other);
	virtual ~HTTPException() throw();

	int getStatusCode() const;
	bool getCloseConnection() const;
};

#endif