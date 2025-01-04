#pragma once
//Definimos aqui cuantos controllers permite nuestro motor utilizar a la vez, de no querer limitarlo se eliminaremos su comprobacion
#define MAX_CONTROLLERS 2


// NONE nos ayuda cuando se pulse una tecla que no sea ninguna de las definidas para el input
// Saltará entonces la busqueda de controles
enum class LoginType { LOGIN_F1, LOGIN_F2, LOGIN_JOYSTICK, NONE };
