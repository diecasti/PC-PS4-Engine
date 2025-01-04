#pragma once
#include <memory>

// Estructura del estado del input
struct InputStatus {

	// Utilizamos float en los ejes porque para PC y PS4 se utilizan distintos tipos

	// Estado de los dos ejes analógicos del stick izquierdo
	float horizontalAxis = 0;	// también son las flechas izda. y dcha. para PC
	float verticalAxis = 0;	// también son las flechas arriba y abajo para PC

	// Estado analógico de cada uno de los dos gatillos (L2 y R2).
	float leftTrigger = 0;
	float rightTrigger = 0;

	// Estado de los botones (X) y (O)
	bool action_1 = 0; // también es el click izdo. (o Mayús. dcho.) para PC
	bool action_2 = 0; // también es el click dcho. (o Ctrl. dcho.) para PC

	// Velocidad angular del dispositivo detectada por el giróscopo
	float angularVelX = 0; // también es el 'delta h.' del ratón para PC
	float angularVelY = 0; // también es el 'delta v.' del ratón para PC
	float angularVelZ = 0; //
};