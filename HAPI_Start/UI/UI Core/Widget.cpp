#include "Widget.h"

#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"

void Widget::HideWidget(ECS_Engine& ecs, RenderManager& renderManager)
{
	std::shared_ptr<RenderComponent> renderComponent{ ecs.GetComponent<RenderComponent>(this->m_entityID) };
	renderManager.RemoveEntityIdFromRenderLayer(this->m_entityID, renderComponent->m_renderLayer);
}

void Widget::ShowWidget(ECS_Engine& ecs, RenderManager& renderManager)
{
	std::shared_ptr<RenderComponent> renderComponent{ ecs.GetComponent<RenderComponent>(this->m_entityID) };
	renderManager.AddEntityIdToRenderLayer(ecs, this->m_entityID, renderComponent->m_renderLayer);
}

