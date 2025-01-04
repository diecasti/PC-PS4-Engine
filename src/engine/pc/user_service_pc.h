#pragma once
#ifndef USERSERVICEPC_H
#define USERSERVICEPC_H

#include <string>
#include <memory>

#include "input_status.h"
#include "SDLEventListener.h"
#include "SDL_gamecontroller.h"
#include <vector>
#include <queue>
#include "player_color.h"
#include "userid_def.h"
#include <unordered_map>

enum class LoginType;

class UserServicePC : public SDLEventListener {
public:
	UserServicePC();
	~UserServicePC();

	static UserServicePC* Instance();

	/// <summary>
	/// Inicializacion de la instancia UserService
	/// </summary>
	/// <returns> False si falla alguna inicializacion, True en caso contrario </returns>
	static bool Init();
	/// Cierra la instancia
	static void Release();

	/// Limpia los vectores de conectados y desconectados
	/// Se debe llamar después de gestionar estas listas en la lógica, después del Input
	void tick();

	/// <returns> Color de usuario según el UserId dado. PlayerColor::NONE si no se ha encontrado el usuario </returns>
	PlayerColor getUIdColor(UserId uid);

	/// Métodos para recorrer las diferentes listas de UserService ///
	std::vector<UserId>::iterator usersBegin();
	std::vector<UserId>::iterator usersEnd();
	const std::vector<UserId>* getInfos();
	const std::vector<UserId>* getConectionInfos();
	const std::vector<UserId>* getDisconectionInfos();
	

	// Necesario en el momento de abrir el mando en Input
	/// <returns> Which de usuario según el UserId dado. -1 si no se ha encontrado el usuario </returns>
	int GetWhichByUid(UserId id);

	// Necesario en el momento de recibir Input, abrir y cerrar puertos
	/// <returns> LoginType de usuario según el UserId dado. LoginType::NONE si no se ha encontrado el usuario </returns>
	LoginType GetLoginTypeByUid(UserId id);

	/// Gestión de los eventos de SDL que añaden o quitan usuarios 
	virtual void processEvent(SDL_Event evt) override;
private:
	//struct con la informacion que contiene UserService, es exclusiov de aqui y no se va autilizar en otra parte
	struct UserIdInfo {
		LoginType type;
		PlayerColor color;
		int which = -1;
		int joyId = -1;
	};

	static std::unique_ptr<UserServicePC> instance;

	bool f1_connected = false;
	bool f2_connected = false;

	// Variable que controla que no haya más de dos mandos en ningún momento
	uint8_t activeControllers = 0;

	// Cola de prioridad que nos permite tener un color único y ordenado para cada usuario
	std::priority_queue<PlayerColor,std::vector<PlayerColor>, std::greater<PlayerColor>> availableColors;
	std::unordered_map<UserId, UserIdInfo> useridInfos;

	// Vectores para que Input y la lógica gestionen los usuarios
	std::vector<UserId> UserIds;
	std::vector<UserId> conectedUserIds;
	std::vector<UserId> disconetedUserIds;

	/// Inicialización interna de UserService
	bool initUserService();

	/// Crea un nuevo UserId y una nueva entrada en useridInfos guardando las variables correspondientes
	void onLogin(LoginType t, int which);

	/// Elimina el UserId del vector UserIds y lo añade a disconnected
	void onLogout(LoginType t, SDL_JoystickID id);

	/// Limpia las lsitas de conexiones una vez que las hayamos checkeado
	void clearConectionList();

	/// Limpia las listas y los mapas de UserService
	/// Se quita como observador de Platform
	void closeUserService();


};
#endif // UserServicePC_H
