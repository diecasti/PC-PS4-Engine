#include "user_service_pc.h"
#include <iostream>

#include "login_type.h"
#include "platform.h"
#include "SDL_events.h"

std::unique_ptr<UserServicePC> UserServicePC::instance = nullptr;

UserServicePC::UserServicePC() = default;

UserServicePC::~UserServicePC() { closeUserService(); }

UserServicePC* UserServicePC::Instance() {
	return instance.get();
}

bool UserServicePC::Init() {
	instance.reset(new UserServicePC());

	if (!instance.get()->initUserService()) {
		instance.reset(nullptr);
		return false;
	}

	return true;
}

void UserServicePC::Release() {
	if (instance.get() == nullptr)
		return;

	instance.reset(nullptr);
}

void UserServicePC::tick()
{
	clearConectionList();
}

PlayerColor UserServicePC::getUIdColor(UserId uid)
{
	if (useridInfos.count(uid))
		return useridInfos[uid].color;
	return PlayerColor::NONE;
}

std::vector<UserId>::iterator UserServicePC::usersBegin()
{
	return UserIds.begin();
}

std::vector<UserId>::iterator UserServicePC::usersEnd()
{
	return UserIds.end();
}

const std::vector<UserId>* UserServicePC::getInfos()
{
	return &UserIds;
}

const std::vector<UserId>* UserServicePC::getConectionInfos()
{
	return &conectedUserIds;
}

const std::vector<UserId>* UserServicePC::getDisconectionInfos()
{
	return &disconetedUserIds;
}

int UserServicePC::GetWhichByUid(UserId id)
{
	if(useridInfos.count(id))
		return useridInfos[id].which;
	return -1;
}

LoginType UserServicePC::GetLoginTypeByUid(UserId id) {
	if (useridInfos.count(id))
		return useridInfos[id].type;
	return LoginType::NONE;
}



void UserServicePC::processEvent(SDL_Event evt)
{
	switch (evt.type) {
	case SDL_CONTROLLERDEVICEADDED:
		if (activeControllers < MAX_CONTROLLERS) {
			activeControllers++;

			onLogin(LoginType::LOGIN_JOYSTICK,
				evt.cdevice.which);
		}
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		onLogout(LoginType::LOGIN_JOYSTICK, evt.cdevice.which);
		break;
	case SDL_KEYDOWN:
		if (evt.key.keysym.sym == SDLK_F1) {
			if (!f1_connected) {
				onLogin(LoginType::LOGIN_F1, -1);
				f1_connected = true;
			}
			else {
				onLogout(LoginType::LOGIN_F1, -1);
				f1_connected = false;
			}
		}
		else if (evt.key.keysym.sym == SDLK_F2) {
			if (!f2_connected) {
				onLogin(LoginType::LOGIN_F2, -1);
				f2_connected = true;
			}
			else {
				onLogout(LoginType::LOGIN_F2, -1);
				f2_connected = false;
			}
		}
		break;
	default:
		break;
	}
}

bool UserServicePC::initUserService()
{
	Platform::Instance()->addListener(this);
	
	availableColors.push(PlayerColor::BLUE);
	availableColors.push(PlayerColor::RED);
	availableColors.push(PlayerColor::GREEN);
	availableColors.push(PlayerColor::PINK);
	return true;
}


void UserServicePC::onLogin(LoginType t, int which)
{


	std::cout << "user connected \n";
	
	UserId u = -1;
	// Para calcular un UserId único para cada jugador, utilizamos la asignación de colores
	switch (availableColors.top())
	{
	case PlayerColor::BLUE:
		u = 0;
		break;
	case PlayerColor::RED:
		u = 1;
		break;
	case PlayerColor::GREEN:
		u = 2;
		break;
	case PlayerColor::PINK:
		u = 3;
		break;
	default:
		break;
	}

	// Con Stress tests nos hemos dado cuenta de que en el mismo frame puede entrar varias veces el mismo usuario
	// Y no queremos que eso sea posible
	// Si el usuario ya está en la lista de conectados no lo conectes otra vez y si está en desconectados borralo
	if (u != -1) {
		useridInfos[u] = UserIdInfo();
		useridInfos[u].color = availableColors.top();
		useridInfos[u].type = t;
		useridInfos[u].which = which;
		useridInfos[u].joyId = SDL_JoystickGetDeviceInstanceID(which);

		UserIds.push_back(u);
		conectedUserIds.push_back(u);
		availableColors.pop();

		auto it = disconetedUserIds.begin();
		while (it != disconetedUserIds.end())
		{
			if ((*it) == u) {
				it = disconetedUserIds.erase(it);
				conectedUserIds.pop_back();
				return;
			}
			else
				++it;
		}
	}
}


void UserServicePC::onLogout(LoginType t, SDL_JoystickID id)
{
	std::cout << "user Loggout \n";
	auto it = UserIds.begin();
	while (it != UserIds.end()) {
		if (useridInfos[(*it)].type == t) {
			// Comprobamos si es un joystick o no para cerrarlo con el which
			if (useridInfos[(*it)].type == LoginType::LOGIN_JOYSTICK) {
				if (useridInfos[(*it)].joyId == id) {
					disconetedUserIds.push_back((*it));
					availableColors.push(useridInfos[(*it)].color);
					
					activeControllers--;

					// Previene que un usuario no pueda estar al mismo tiempo en las colas de conectados y desconectados
					auto it2 = conectedUserIds.begin();
					while (it2 != conectedUserIds.end())
					{
						if ((*it2) == (*it)) {
							it2 = conectedUserIds.erase(it2);
							disconetedUserIds.pop_back();
							useridInfos.erase((*it));
							it = UserIds.erase(it);
							return;
						}
						else
							++it2;
					}

					useridInfos.erase((*it));
					it = UserIds.erase(it);
				}
				else ++it;
			}
			else {
				disconetedUserIds.push_back((*it));
				availableColors.push(useridInfos[(*it)].color);

				auto it2 = conectedUserIds.begin();
				while (it2 != conectedUserIds.end())
				{
					if ((*it2) == (*it)) {
						it2 = conectedUserIds.erase(it2); 
						disconetedUserIds.pop_back();
						useridInfos.erase((*it));
						it = UserIds.erase(it);
						return;
					}
					else
						++it2;
				}
				useridInfos.erase((*it));
				it = UserIds.erase(it);
			}
		}
		else
			++it;
	}
}

// metodo auxiliar para limpiar las listas de comprobacion de conexion antes de cada uno de los ticks
// y dejar las listas preparadas para llenarse con las nuevas conexiones y desconexiones
void UserServicePC::clearConectionList()
{
	conectedUserIds.clear(); //No queremos eliminar ninguna informacion que este alojada aqui, ya que siguen vivos en UserIds

	disconetedUserIds.clear();

}

void UserServicePC::closeUserService()
{
	if (Platform::Instance())
		Platform::Instance()->removeObserver(this);
	UserIds.clear();
	clearConectionList();
}
