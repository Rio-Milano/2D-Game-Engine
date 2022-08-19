#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include<HAPI_lib.h>
using namespace HAPISPACE;

class InputManager final
{
public:
	static InputManager& GetInstance();

	const HAPI_TKeyboardData& GetKeyboardData()const;
	const HAPI_TMouseData& GetMouseData()const;
	const HAPI_TControllerData& GetControllerData()const;


private:

	InputManager();
	~InputManager();

	const HAPI_TKeyboardData& m_keyboardData;
	const HAPI_TMouseData& m_mouseData;
	const HAPI_TControllerData& m_controllerData;

};

#endif