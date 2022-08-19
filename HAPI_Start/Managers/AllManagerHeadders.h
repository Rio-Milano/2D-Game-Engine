#ifndef ALLMANAGERHEADDERS_H
#define ALLMANAGERHEADDERS_H

#include"RenderManager.h"

#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include"InputManager.h"
#define InputManager InputManager::GetInstance()
#define Keyboard InputManager.GetKeyboardData()
#define Mouse InputManager.GetMouseData()
#define Controller InputManager.GetControllerData()


#endif