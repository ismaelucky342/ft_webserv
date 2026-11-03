/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 13:16:00 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/10/27 11:02:50 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HTTPException.hpp"

/** Constructor for HTTPException
 * 
 * statusCode: The HTTP status code associated with the exception.
 * closeConnection: A boolean indicating whether the connection should be closed. True if the connection should be closed, false otherwise.
 * It must be closed if there is an error and we can't clean a complete request of the client buffer, because we don't know where the next request starts yet.
 */
HTTPException::HTTPException(int statusCode, bool closeConnection) : _statusCode(statusCode), _closeConnection(closeConnection) {}

/** Copy constructor for HTTPException
 * 
 * other: The HTTPException object to copy.
 */
HTTPException::HTTPException(const HTTPException &other) : _statusCode(other._statusCode), _closeConnection(other._closeConnection) {}

/** Assignment operator for HTTPException
 * 
 * other: The HTTPException object to assign.
 * @return A reference to the assigned object.
 */
HTTPException &HTTPException::operator=(const HTTPException &other)
{
	if (this != &other)
	{
		_statusCode = other._statusCode;
		_closeConnection = other._closeConnection;
	}
	return *this;
}

/** Destructor for HTTPException
 */
HTTPException::~HTTPException() throw() {}

/** Returns the HTTP status code associated with the exception.
 * 
 * @return The HTTP status code.
 */
int HTTPException::getStatusCode() const
{
	return _statusCode;
}

/** Returns the boolean indicating whether the connection should be closed after the exception.
 * 
 * @return A boolean indicating whether the connection should be closed.
 */
bool HTTPException::getCloseConnection() const
{
	return _closeConnection;
}