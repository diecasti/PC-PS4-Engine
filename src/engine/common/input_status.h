#pragma once
#include <memory>

// Estructura del estado del input
struct InputStatus {

	// Utilizamos float en los ejes porque para PC y PS4 se utilizan distintos tipos

	// Estado de los dos ejes anal�gicos del stick izquierdo
	float horizontalAxis = 0;	// tambi�n son las flechas izda. y dcha. para PC
	float verticalAxis = 0;	// tambi�n son las flechas arriba y abajo para PC

	// Estado anal�gico de cada uno de los dos gatillos (L2 y R2).
	float leftTrigger = 0;
	float rightTrigger = 0;

	// Estado de los botones (X) y (O)
	bool action_1 = 0; // tambi�n es el click izdo. (o May�s. dcho.) para PC
	bool action_2 = 0; // tambi�n es el click dcho. (o Ctrl. dcho.) para PC

	// Velocidad angular del dispositivo detectada por el gir�scopo
	float angularVelX = 0; // tambi�n es el 'delta h.' del rat�n para PC
	float angularVelY = 0; // tambi�n es el 'delta v.' del rat�n para PC
	float angularVelZ = 0; //
};