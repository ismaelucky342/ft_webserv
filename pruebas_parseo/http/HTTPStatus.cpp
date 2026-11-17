/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal-h <mvidal-h@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 12:51:31 by mvidal-h          #+#    #+#             */
/*   Updated: 2026/11/17 16:02:57 by mvidal-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"

/**
 * Returns the status message for a given HTTP status code.
 * 
 * status: The HTTP status code.
 * @return The corresponding status message.
 */
std::string getStatusMessage(HTTPStatus status)
{
	switch (status)
	{
	case OK:
		return "OK";
	case CREATED:
		return "Created";
	case NO_CONTENT:
		return "No Content";
	case MOVED_PERMANENTLY:
		return "Moved Permanently";
	case BAD_REQUEST:
		return "Bad Request";
	case FORBIDDEN:
		return "Forbidden";
	case NOT_FOUND:
		return "Not Found";
	case METHOD_NOT_ALLOWED:
		return "Method Not Allowed";
	case REQUEST_TIMEOUT:
		return "Request Timeout";
	case LENGTH_REQUIRED:
		return "Length Required";
	case CONTENT_TOO_LARGE:
		return "Content Too Large";
	case URI_TOO_LONG:
		return "URI Too Long";
	case INTERNAL_SERVER_ERROR:
		return "Internal Server Error";
	case NOT_IMPLEMENTED:
		return "Not Implemented";
	case SERVICE_UNAVAILABLE:
		return "Service Unavailable";
	case HTTP_VERSION_NOT_SUPPORTED:
		return "HTTP Version Not Supported";
	default:
		return "Unknown Status Code";
	}
}

std::string getErrorDescription(HTTPStatus status)
{
	switch (status)
	{
		case BAD_REQUEST:
			return "La solicitud enviada al servidor no es valida.";
		case FORBIDDEN:
			return "No tienes permiso para acceder a este recurso.";
		case NOT_FOUND:
			return "El servidor no ha encontrado el recurso solicitado.";
		case METHOD_NOT_ALLOWED:
			return "El metodo HTTP utilizado no esta permitido para este recurso.";
		case REQUEST_TIMEOUT:
			return "El servidor ha tardado demasiado en recibir la solicitud.";
		case LENGTH_REQUIRED:
			return "La solicitud debe incluir la cabecera Content-Length.";
		case CONTENT_TOO_LARGE:
			return "El contenido de la solicitud supera el tamano maximo permitido.";
		case URI_TOO_LONG:
			return "La URI solicitada es demasiado larga.";
		case INTERNAL_SERVER_ERROR:
			return "Se ha producido un error interno en el servidor.";
		case NOT_IMPLEMENTED:
			return "El servidor no admite la funcionalidad solicitada.";
		case SERVICE_UNAVAILABLE:
			return "El servidor no esta disponible temporalmente.";
		case HTTP_VERSION_NOT_SUPPORTED:
			return "La version HTTP utilizada no esta soportada.";
		default:
			return "Se ha producido un error al procesar la solicitud.";
	}
}

HTTPStatus stringToHTTPStatus(const std::string &statusString)
{
	if (statusString == "200")
		return OK;
	else if (statusString == "201")
		return CREATED;
	else if (statusString == "204")
		return NO_CONTENT;
	else if (statusString == "301")
		return MOVED_PERMANENTLY;
	else if (statusString == "400")
		return BAD_REQUEST;
	else if (statusString == "403")
		return FORBIDDEN;
	else if (statusString == "404")
		return NOT_FOUND;
	else if (statusString == "405")
		return METHOD_NOT_ALLOWED;
	else if (statusString == "408")
		return REQUEST_TIMEOUT;
	else if (statusString == "411")
		return LENGTH_REQUIRED;
	else if (statusString == "413")
		return CONTENT_TOO_LARGE;
	else if (statusString == "414")
		return URI_TOO_LONG;
	else if (statusString == "500")
		return INTERNAL_SERVER_ERROR;
	else if (statusString == "501")
		return NOT_IMPLEMENTED;
	else if (statusString == "503")
		return SERVICE_UNAVAILABLE;
	else if (statusString == "505")
		return HTTP_VERSION_NOT_SUPPORTED;
	throw std::runtime_error("Invalid HTTP status code: " + statusString);
}