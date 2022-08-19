#include "InputManager.h"



InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}


InputManager::InputManager()
    : m_keyboardData(HAPI.GetKeyboardData()),
    m_mouseData(HAPI.GetMouseData()),
    m_controllerData(HAPI.GetControllerData(0))
{}

InputManager::~InputManager()
{
}

const HAPI_TKeyboardData& InputManager::GetKeyboardData() const
{
    return m_keyboardData;
}

const HAPI_TMouseData& InputManager::GetMouseData() const
{
   //not working return m_mouseData;
    return HAPI.GetMouseData();
}

const HAPI_TControllerData& InputManager::GetControllerData() const
{
    //not working
    //return m_controllerData;
    return HAPI.GetControllerData(0);
}
