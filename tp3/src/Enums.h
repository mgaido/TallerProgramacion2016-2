/*
 * Enums.h
 *
 *  Created on: 6 dic. 2016
 *      Author: Rod
 */

#ifndef ENUMS_H_
#define ENUMS_H_

#include <string>

enum class Tipo {  GunH, GunC, GunS, GunR, //Proyectiles jugador
					Bomba, Misil, Laser, Torreta, //Proyectiles bosses
					Jugador, Enemigo, EnemigoFuerte, Boss1, Boss2, Boss3, //Personajes
					BonusVida, BonusKill, BonusArmaH, BonusArmaS, BonusArmaR, BonusArmaC, //PickUps
					Plataforma, Efecto,
					Void
			};

enum class Estado { Normal, Caminando, Saltando, Desconectado, Efecto };

enum class EstadoJuego { NoIniciado, EnJuego, Perdido, Ganado };


inline bool esEnemigo(Tipo tipo){
	return tipo == Tipo::Enemigo || tipo == Tipo::EnemigoFuerte ||tipo == Tipo::Boss1 || tipo == Tipo::Boss2 || tipo == Tipo::Boss3;
}

inline std::string nombreTipo(Tipo tipo) {
	std::string str;
	switch (tipo) {
	case Tipo::GunH:
		str = "GunH";
		break;
	case Tipo::GunC:
		str = "GunC";
		break;
	case Tipo::GunS:
		str = "GunS";
		break;
	case Tipo::GunR:
		str = "GunR";
		break;
	case Tipo::Bomba:
		str = "Bomba";
		break;
	case Tipo::Misil:
		str = "Misil";
		break;
	case Tipo::Laser:
		str = "Laser";
		break;
	case Tipo::Torreta:
		str = "Torreta";
		break;
	case Tipo::Jugador:
		str = "Jugador";
		break;
	case Tipo::Enemigo:
		str = "Enemigo";
		break;
	case Tipo::EnemigoFuerte:
		str = "EnemigoFuerte";
		break;
	case Tipo::Boss1:
		str = "Boss1";
		break;
	case Tipo::Boss2:
		str = "Boss2";
		break;
	case Tipo::Boss3:
		str = "Boss3";
		break;
	case Tipo::BonusVida:
		str = "BonusVida";
		break;
	case Tipo::BonusKill:
		str = "BonusKill";
		break;
	case Tipo::BonusArmaH:
		str = "BonusArmaH";
		break;
	case Tipo::BonusArmaS:
		str = "BonusArmaS";
		break;
	case Tipo::BonusArmaR:
		str = "BonusArmaR";
		break;
	case Tipo::BonusArmaC:
		str = "BonusArmaC";
		break;
	case Tipo::Plataforma:
		str = "Plataforma";
		break;
	default:
		str = "????";
	}
	return str;
}

inline Tipo tipoPorNombre(std::string nombre) {
	Tipo tipo = Tipo::Void;

	if (nombre == "GunH")
		tipo = Tipo::GunH;

	if (nombre == "GunC")
		tipo = Tipo::GunC;

	if (nombre == "GunS")
		tipo = Tipo::GunS;

	if (nombre == "GunR")
		tipo = Tipo::GunR;

	if (nombre == "Bomba")
		tipo = Tipo::Bomba;

	if (nombre == "Misil")
		tipo = Tipo::Misil;

	if (nombre == "Laser")
		tipo = Tipo::Laser;

	if (nombre == "Torreta")
		tipo = Tipo::Torreta;

	if (nombre == "Jugador")
		tipo = Tipo::Jugador;

	if (nombre == "Enemigo")
		tipo = Tipo::Enemigo;

	if (nombre == "EnemigoFuerte")
		tipo = Tipo::EnemigoFuerte;

	if (nombre == "Boss1")
		tipo = Tipo::Boss1;

	if (nombre == "Boss2")
		tipo = Tipo::Boss2;

	if (nombre == "Boss3")
		tipo = Tipo::Boss3;

	if (nombre == "BonusVida")
		tipo = Tipo::BonusVida;

	if (nombre == "BonusKill")
		tipo = Tipo::BonusKill;

	if (nombre == "BonusArmaH")
		tipo = Tipo::BonusArmaH;

	if (nombre == "BonusArmaS")
		tipo = Tipo::BonusArmaS;

	if (nombre == "BonusArmaR")
		tipo = Tipo::BonusArmaR;

	if (nombre == "BonusArmaC")
		tipo = Tipo::BonusArmaC;

	if (nombre == "Plataforma")
		tipo = Tipo::Plataforma;

	return tipo;
}

class Punto {
public:
	int x;
	int y;
	Punto(){
		x=0;
		y=0;
	}
};


#endif /* ENUMS_H_ */
