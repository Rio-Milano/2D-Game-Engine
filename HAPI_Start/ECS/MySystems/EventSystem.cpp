#include "EventSystem.h"

#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Core/UI_Manager.h"

EventSystem::EventSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<EventComponent>();
}

void EventSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
	//we may not have an event component
	std::shared_ptr<EventComponent> eventComponent{ecs.GetComponent<EventComponent>(entityID)};
	if (!eventComponent)
	{
		std::cerr << std::string("The event component could not be retrived, Entity :" + entityID);
		return;
	}

	eventComponent->m_isMouseLeftClick = false;
	eventComponent->m_isMouseOver = false;     
	eventComponent->m_tagComponent = false;
}

void EventSystem::EndOfSystemUpdate()
{
	if (this->m_hasEngagedWithEntity)
		this->m_hasFoundEntity = true;

	if (!Mouse.leftButtonDown)
	{
		this->m_hasEngagedWithEntity = false;
		this->m_hasFoundEntity = false;
	}
}

void EventSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{	
	//we will have an event component
	std::shared_ptr<EventComponent> eventComponent{ ecs.GetComponent<EventComponent>(entityID) };

	std::shared_ptr<RenderComponent>
		entityIDRenderComponent,
		UI_EngagedRenderComponent;

		entityIDRenderComponent = ecs.GetComponent<RenderComponent>(entityID);
		UI_EngagedRenderComponent = ecs.GetComponent<RenderComponent>(UI_Manager::GetEntityEngaged());

	const int32_t& UI_engagedTo_Entity{UI_Manager::GetEntityEngaged()};

	if (UI_engagedTo_Entity == -1 ||
		UI_engagedTo_Entity == entityID ||
		eventComponent->parentID == UI_engagedTo_Entity ||
		!m_hasFoundEntity && (
		(UI_engagedTo_Entity != -1  && entityIDRenderComponent->m_renderLayer > UI_EngagedRenderComponent->m_renderLayer) ||
		
			(
				UI_engagedTo_Entity != -1 &&
			entityIDRenderComponent->m_renderLayer == UI_EngagedRenderComponent->m_renderLayer && 
			entityIDRenderComponent->m_renderLayerPrecedence > UI_EngagedRenderComponent->m_renderLayerPrecedence
			)
		))
	{
		Vector2i mousePos{ InputManager.GetMouseData().x, InputManager.GetMouseData().y };

		std::shared_ptr<RenderComponent> renderComponent{ ecs.GetComponent<RenderComponent>(entityID) };

		Vector2i offsetFromCamera{ 0, 0 };

		if (renderComponent->m_WorldRender)
			offsetFromCamera = ecs.GetComponent<TransformComponent>(ecs.GetTaggedEntities("Camera")[0])->position.ToType<int>();

		if (eventComponent->m_eventRect.IsPointLyingOnRect(mousePos + offsetFromCamera))
		{
			eventComponent->m_isMouseOver = true;
			if (eventComponent->parentID != UI_engagedTo_Entity)
			{
				//reset the bind attributes
				std::shared_ptr<EventComponent> other{ ecs.GetComponent<EventComponent>(UI_Manager::GetEntityEngaged()) };
				if(other && UI_Manager::GetEntityEngaged() != entityID)
				{
					other->m_isMouseOver = false;
					other->m_isMouseLeftClick = false;
					other->m_mouseLeftClickHolding = false;
				}
				this->m_hasEngagedWithEntity = true;
				UI_Manager::SetEntityEngaged(static_cast<int32_t>(entityID));
			}

			if (InputManager.GetMouseData().leftButtonDown)
			{
				if (eventComponent->m_isMouseLeftClick)
					eventComponent->m_mouseLeftClickHolding = true;

				eventComponent->m_isMouseLeftClick = true;
			}
			else
			{
				eventComponent->m_isMouseLeftClick = false;
				eventComponent->m_mouseLeftClickHolding = false;
			}
		}
		else
		{
			if (!Mouse.leftButtonDown)
			{
				eventComponent->m_isMouseOver = false;
				eventComponent->m_isMouseLeftClick = false;
				eventComponent->m_mouseLeftClickHolding = false;

				if (UI_engagedTo_Entity == entityID)
					UI_Manager::SetEntityEngaged(-1);
			}

		}
	}
	

	/*`
	Move rect to transform
	*/

	//we may not have a transform
	std::shared_ptr<TransformComponent> eventTransform{ecs.GetComponent<TransformComponent>(entityID)};
	if (eventTransform == nullptr)
	{
		std::cerr << std::string("Transform component was needed to perform an update on an event component!\n");
		return;
	}
	
	eventComponent->m_eventRect.Translate(eventTransform->position.ToType<int>());
}

std::shared_ptr<BaseComponent> EventSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	//we may not have an event component
	std::shared_ptr<EventComponent> toClone{std::static_pointer_cast<EventComponent>(m_componentEngine.GetComponent(entityID))};
	if (toClone == nullptr)
	{
		std::cerr << std::string("Cant clone Event component because it dosent exist, Entity :" + std::to_string(entityID));
		return nullptr;
	}



	std::shared_ptr<EventComponent> duplicatedComponent = forignECS.AddComponent<EventComponent>(forignEntityID);

	duplicatedComponent->InitializeEventComponent(
		toClone->m_eventRect
	);

	return duplicatedComponent;

}
