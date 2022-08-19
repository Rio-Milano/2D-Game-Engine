#include "System.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../Managers/RenderManager.h"
void System::EnableSystem(ECS_Engine& ecs)
{
	this->m_systemState = true;
	SystemEnableProdecure(ecs);
}

void System::DisableSystem(ECS_Engine& ecs)
{
	this->m_systemState = false;
	SystemDisableProcedure(ecs);
}

ComponentEngine& System::GetComponentEngine()
{
	return m_componentEngine;
}
