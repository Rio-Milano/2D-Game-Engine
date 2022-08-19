
#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include<HAPI_Types.h>
using namespace HAPISPACE;

#include"../Core/Renderer.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../DataStructures/Vector.h"

class RenderManager
{
public:

	RenderManager();
	~RenderManager() = default;

	//when not using RGBA the function will render to monochrome color using the R channel 
	void Render_RenderComponents(ECS_Engine& ecs, Renderer& renderer, const Vector2i& cameraOffset);

	void AddEntityIdToRenderLayer(ECS_Engine& ecs, const uint32_t& entityID, const size_t& layer);
	void RemoveEntityIdFromRenderLayer(const uint32_t& entityID, const size_t& layer);//todo

	void RemoveDeletedEntitiesFromRenderLayers(ECS_Engine& ecs);

	void RecalculateGenericRenderLayerPrecedenceCount(ECS_Engine& ecs);

	static Vector2f mousePositionInWorldForTestingAIPathfindingInDevTesting;
private:

	std::vector<std::list<uint32_t>> m_renderLayers;

	uint32_t m_genericRenderLayerPrecedence{0};

};

#endif