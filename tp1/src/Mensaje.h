/*
 * Mensaje.h
 *
 *  Created on: 4 sep. 2016
 *      Author: rodrigo
 */

#ifndef MENSAJE_H_
#define MENSAJE_H_

#include "stdafx.h"
#include "Usuario.h"


class Mensaje {
public:
	Mensaje();
	virtual ~Mensaje();

	const Usuario& getDestinatario() const;
	void setDestinatario(const Usuario& destinatario);

	const Usuario& getRemitente() const;
	void setRemitente(const Usuario& remitente);

	std::string getTexto() const;
	void setTexto(std::string texto);

private:
	Usuario remitente;
	Usuario destinatario;
	std::string texto;
};

#endif /* MENSAJE_H_ */
