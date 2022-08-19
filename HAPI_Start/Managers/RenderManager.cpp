#include "RenderManager.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Scenes/SceneCore/Scene.h"
#include"../GeneralObjects/Node.h"
#include<unordered_map>
#include<algorithm>
#include<iostream>
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

Vector2f RenderManager::mousePositionInWorldForTestingAIPathfindingInDevTesting{};


RenderManager::RenderManager()
{
	//init render layer size to 5 elements
	m_renderLayers.resize(RenderComponent::RenderLayer::DoNotUse_TotalRenderLayers);

}

void RenderManager::Render_RenderComponents(ECS_Engine& ecs, Renderer& renderer, const Vector2i& cameraOffset)
{
	//clean data store 
	//now done in ecs update RemoveDeletedEntitiesFromRenderLayers(ecs);

	std::shared_ptr<SpriteComponent>spriteComponent{ nullptr };
	std::shared_ptr<TransformComponent> transformComponent{ nullptr };
	std::shared_ptr<CollisionComponent> collisionComponent{ nullptr };//we will allways have a valid render component when iterating
	std::shared_ptr<RenderComponent> renderComponent{ nullptr };//we will allways have a valid render component when iterating
	std::shared_ptr<EventComponent> eventComponent{nullptr};



	for (std::list<uint32_t>& layer : m_renderLayers)
	{
		for (const uint32_t& entityID : layer)
		{
			renderComponent = ecs.GetComponent<RenderComponent>(entityID);
			if (!renderComponent)
			{
				std::cerr << std::string("RenderComponent for entity :" + std::to_string(entityID) + " was not found!\n");
				continue;
			}
			
			if (!renderComponent->m_isActive)
				continue;

			if (renderComponent->m_onlyRenderWhenInEditorMode)
			{
				if (!Scene::m_editorMode)
					continue;
			}


			//if we are rendering anything
			if(renderComponent->m_renderConfigs)
			{
				transformComponent = ecs.GetComponent<TransformComponent>(entityID);

				if (!transformComponent)
				{
					std::cerr << "Transform Component of entity :" + std::to_string(entityID) + " was not found\n";
					continue;
				}


				if (renderComponent->m_renderConfigs & RenderComponent::Sprite)
				{
					spriteComponent = ecs.GetComponent<SpriteComponent>(entityID);
					if (!spriteComponent)
					{
						std::cerr << "Sprite Component of entity :" + std::to_string(entityID) + " was not found\n";
						continue;
					}
					Vector2i transformPosInt{transformComponent->position.ToType<int>()};
					if(renderComponent->m_WorldRender)
						renderer.DrawTexture(transformPosInt + cameraOffset, spriteComponent->m_sprite);
					else
						renderer.DrawTexture(transformPosInt, spriteComponent->m_sprite);
				}


				if (renderComponent->m_renderConfigs & RenderComponent::DrawBoundingBox)
				{
					if (renderComponent->m_renderConfigs & RenderComponent::RenderConfigs::DrawBoundingBoxCollisionComponent)
					{
						collisionComponent = ecs.GetComponent<CollisionComponent>(entityID);
						if (!collisionComponent)
						{
							std::cerr << "Collision Component of entity :" + std::to_string(entityID) + " was not found\n";
							return;
						}
						Vector2i collisionPosInt{ collisionComponent->m_Rect.GetPosition().ToType<int>() };
						if (renderComponent->m_WorldRender)
							collisionComponent->m_Rect.DebugDrawRect(collisionPosInt + cameraOffset, collisionComponent->m_Rect.m_debugRectColor, renderer, false, false);
						else
							collisionComponent->m_Rect.DebugDrawRect(collisionPosInt, collisionComponent->m_Rect.m_debugRectColor, renderer, false, false);
					}
					if(renderComponent->m_renderConfigs & RenderComponent::RenderConfigs::DrawBoundBoxEventComponent)
					{
						eventComponent = ecs.GetComponent<EventComponent>(entityID);
						if (!eventComponent)
						{
							std::cerr << "Event Component of entity :" + std::to_string(entityID) + " was not found\n";
							continue;
						}
						Vector2i eventRectPos{ eventComponent->m_eventRect.GetPosition() };
						if (renderComponent->m_WorldRender)
							eventComponent->m_eventRect.DebugDrawRect(eventRectPos + cameraOffset, eventComponent->m_eventRect.m_debugRectColor, renderer, false, false);
						else
							eventComponent->m_eventRect.DebugDrawRect(eventRectPos, eventComponent->m_eventRect.m_debugRectColor, renderer, false, false);

					}
				}

				if (renderComponent->m_renderConfigs & RenderComponent::Text)
				{
					std::shared_ptr<TextComponent> textComponent{ecs.GetComponent<TextComponent>(entityID)};
					if (!textComponent)
					{
						std::cerr << "Text Component of entity :" + std::to_string(entityID) + " was not found\n";
						continue;
					}

					//if not clipping text to a rect
					if (!(renderComponent->m_renderConfigs & RenderComponent::RenderConfigs::ClipTextToEventRect))
					{
						Vector2i transformPosInt{ transformComponent->position.ToType<int>() };
						if (renderComponent->m_WorldRender)
							renderer.DrawText(transformPosInt + cameraOffset + textComponent->m_offsetFromTransform.ToType<int>(), textComponent->m_fillColour, textComponent->m_text, textComponent->m_textSize, textComponent->m_styleFlags);
						else
							renderer.DrawText(transformPosInt + textComponent->m_offsetFromTransform.ToType<int>(), textComponent->m_fillColour, textComponent->m_text, textComponent->m_textSize, textComponent->m_styleFlags);
					}
					//if clipping text
					else
					{
						//get the event rect of the entity to clip text to
						const IntRect& otherEntityEventComponentClip{ecs.GetComponent<EventComponent>(textComponent->clipToEntityEventRectID)->m_eventRect};

						Vector2i transformPosInt{ transformComponent->position.ToType<int>() };
						
						if (renderComponent->m_WorldRender)
							renderer.DrawTextClip(transformPosInt + cameraOffset + textComponent->m_offsetFromTransform.ToType<int>(), textComponent->m_fillColour, HAPI_TColour::ZERO, 0.0f, textComponent->m_text,
								textComponent->m_textSize, textComponent->m_styleFlags, otherEntityEventComponentClip.x1, otherEntityEventComponentClip.x2, otherEntityEventComponentClip.y1,
								otherEntityEventComponentClip.y2);
						
						else
							renderer.DrawTextClip(transformPosInt + textComponent->m_offsetFromTransform.ToType<int>(), textComponent->m_fillColour, HAPI_TColour::ZERO, 0.0f, textComponent->m_text,
								textComponent->m_textSize, textComponent->m_styleFlags, otherEntityEventComponentClip.x1, otherEntityEventComponentClip.x2, otherEntityEventComponentClip.y1,
								otherEntityEventComponentClip.y2);

					}
				}
			}

			static bool hasMadeHealthBarTexture{false};
			static std::shared_ptr<Texture> healthBarTexture;

			if (!hasMadeHealthBarTexture)
			{
				hasMadeHealthBarTexture = true;
				BYTE* texturePtr{ nullptr };
				Vector2i textureSize{ 60, 8 };
				TextureManager.FillNullTextureBuffer(&texturePtr, textureSize, 4);
				healthBarTexture = TextureManager.MakeTexture(texturePtr, textureSize);
			}

			std::shared_ptr<LifeComponent> lifeComponent{ ecs.GetComponent<LifeComponent>(entityID) };
			if (lifeComponent != nullptr)
			{
				if (lifeComponent->m_drawHealthBar)
				{
					TextureManager.FillBufferWithColor(healthBarTexture->m_textureBuffer, healthBarTexture->m_textureSize, HAPI_TColour::GREEN);
					
					//lost health over
					if (lifeComponent->m_maxHealth > 0)
					{
						int Xoverride{ static_cast<int>((Utils::Max<int>(static_cast<int>(lifeComponent->m_health), 0) / lifeComponent->m_maxHealth) * healthBarTexture->m_textureSize.x) };

						BYTE* tempTexture{ healthBarTexture->m_textureBuffer + Xoverride * 4 };
						for (int y{ 0 }; y < healthBarTexture->m_textureSize.y; y++)
						{
							for (int x{ Xoverride }; x < healthBarTexture->m_textureSize.x; x++)
							{
								memcpy(tempTexture, &HAPI_TColour::RED, 4);
								tempTexture += 4;

							}
							tempTexture += Xoverride * 4;
						}

						Sprite package;
						package.InitializeSprite(healthBarTexture, true, Effects::NoEffects);

						std::shared_ptr<TransformComponent> transformComponent{ ecs.GetComponent<TransformComponent>(entityID) };

						Vector2i entityTransformPosition{ transformComponent->position.ToType<int>() };
						entityTransformPosition.y -= 10;
						renderer.DrawTexture(entityTransformPosition + cameraOffset, package);
					}
				}
			}

		}
	}

	//draw AI nodes
	static bool drawAIGraphNodes{!true};
	if (drawAIGraphNodes)
	{
		std::shared_ptr<AISystem> aiSystem{ ecs.GetSystem<AISystem>() };
		for (const std::vector<std::shared_ptr<Node>>& row : aiSystem->GetNodeGraph())
		{
			for (const std::shared_ptr<Node>& node : row)
			{
				renderer.SetPixel(node->m_worldPosition.ToType<int>() + cameraOffset, HAPI_TColour::WHITE, false, false);
			}
		}
	}
};


void RenderManager::AddEntityIdToRenderLayer(ECS_Engine& ecs, const uint32_t& entityID, const size_t& layer)
{
	if (layer >= m_renderLayers.size())
	{
		std::cerr << std::string("Entity :" + std::to_string(entityID) + " is trying to log itself to a non existant render layer!\n");
		return;
	}

	std::shared_ptr<RenderComponent> renderComponent{ecs.GetComponent<RenderComponent>(entityID)};
	if (!renderComponent)
	{
		std::cerr << std::string("Render Component was not found for entity :" + std::to_string(entityID));
		return;
	}

	//if the entity dosent have a render layer precedence
	if (renderComponent->m_renderLayerPrecedence == -1)
	{
		//assign one
		renderComponent->m_renderLayerPrecedence = this->m_genericRenderLayerPrecedence++;
		m_renderLayers[layer].push_back(entityID);
	}
	else
	{
	

		m_renderLayers[layer].push_back(entityID);
		
		/*
		There may be an issue with the sort function as the last element in the list is not sorted for some reason...
		*/
		m_renderLayers[layer].sort(
			[&](const uint32_t& lhs_entityID, const uint32_t& rhs_entityID)->bool
			{
				return ecs.GetComponent<RenderComponent>(lhs_entityID)->m_renderLayerPrecedence <
					ecs.GetComponent<RenderComponent>(rhs_entityID)->m_renderLayerPrecedence;
			});
		
	}

}

void RenderManager::RemoveEntityIdFromRenderLayer(const uint32_t& entityID, const size_t& layer)
{
	if (layer + 1 > this->m_renderLayers.size())
		return;

	std::list<uint32_t>::iterator elementItr{std::find(this->m_renderLayers[layer].begin(), this->m_renderLayers[layer].end(), entityID)};

	if (elementItr != this->m_renderLayers[layer].end())
		this->m_renderLayers[layer].erase(elementItr);
}

void RenderManager::RemoveDeletedEntitiesFromRenderLayers(ECS_Engine& ecs)
{
	//get a ref to deleted entity queue
	std::list<uint32_t>& entitiesQueuedToBeDeletedRef{ ecs.GetEntitiesQueuedToBeDeleted() };

	//if deleted entities present
	if (entitiesQueuedToBeDeletedRef.size() > 0)
	{
		//define render component
		std::shared_ptr<RenderComponent> renderComponent{nullptr};

		//define iterator to loop through deleted entities
		std::list<uint32_t>::iterator deletedEntityID{ entitiesQueuedToBeDeletedRef.begin()};
		//loop through deleted entities
		for(; deletedEntityID != entitiesQueuedToBeDeletedRef.end(); ++deletedEntityID)
		{
			//get the render component for the current entitity
			renderComponent = ecs.GetComponent<RenderComponent>(*deletedEntityID);
			//if the render component is valid
			if (renderComponent != nullptr)
			{
				uint32_t renderLayerIndexOfEntity{ renderComponent->m_renderLayer };

				std::list<uint32_t>& renderLayer{this->m_renderLayers[renderLayerIndexOfEntity]};

				std::list<uint32_t>::iterator entityIter{ std::find(renderLayer.begin(), renderLayer.end(), *deletedEntityID) };
				
				//if the deleted entityID is in the alleged render layer
				if (entityIter != renderLayer.end())
					renderLayer.erase(entityIter);
			}
		}
	}
}

void RenderManager::RecalculateGenericRenderLayerPrecedenceCount(ECS_Engine& ecs)
{
	m_genericRenderLayerPrecedence = 0;

	for (const std::list<uint32_t>& renderLayer : this->m_renderLayers)
	{
		for (const uint32_t& layerElement : renderLayer)
			ecs.GetComponent<RenderComponent>(layerElement)->m_renderLayerPrecedence = m_genericRenderLayerPrecedence++;
	}

}

