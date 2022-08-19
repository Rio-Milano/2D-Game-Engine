#include"Utils.h"

#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/RenderManager.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../Scenes/MyScenes/SaveSelectionScene.h"
#include"../ECS/Base/Core_ECS_Assets.h"


#include<HAPI_Types.h>
using namespace HAPISPACE;


void Utils::LoadLevel
(
	bool& saveLoaded,
	std::string& fileName,
	const SaveSelection& saveSelection,
	ECS_Engine& localEcs,
	ECS_Engine& prefabEcs,
	RenderManager& renderManager,
	UI_Manager& uiManager
)
{
	if (!saveLoaded)
	{
		fileName = "Data\\XML\\";
		if (saveSelection == SaveSelection::Save1)
			fileName += "save1";
		else if (saveSelection == SaveSelection::Save2)
			fileName += "save2";
		else
			fileName += "save3";
		fileName += ".xml";
		if (!Utils::Load_XML_Into_ECS(fileName, localEcs, prefabEcs, renderManager))
			std::cerr << std::string("Failed to load xml file: " + fileName);
		saveLoaded = true;

		localEcs.UpdateECS(0.0f, renderManager, uiManager, prefabEcs);

		//load AI for the level
		std::shared_ptr<AISystem> aiSystem{localEcs.GetSystem<AISystem>()};
		aiSystem->ClearGraph();
		aiSystem->InitialisePathfindingGraph(128, localEcs.GetSystem<CollisionSystem>());
	}
}

/*
Update to this function:
	-> Entities only get saved to an xml file if their tag name is found in the prefab engine
	-> This allows me to focus on placable items like tiles, enemies, spawn points, check points and other things
*/
bool Utils::SaveEcsToXML(const std::string& xmlFileNameArg, ECS_Engine& ecsArg, ECS_Engine& prefabEcsArg)
{
	/*
	Note to self -> Look into XML node types and values
	*/

	CHapiXMLNode* xmlRootNode{ new CHapiXMLNode("Entities") };

	for (const std::pair<std::string, std::vector<uint32_t>>& tagNameToEntityList : ecsArg.m_ECS_TaggedEntity_IDs)
	{
		if (prefabEcsArg.GetTaggedEntities(tagNameToEntityList.first).size() == 0)
			continue;
		int currentInstanceNum{ 0 };

		CHapiXMLNode* xmlEntityTypeNode{ new CHapiXMLNode(tagNameToEntityList.first) };
		xmlRootNode->AddChild(xmlEntityTypeNode);

		for (const uint32_t& entityID : tagNameToEntityList.second)
		{
			CHapiXMLNode* xmlEntityInstanceNode{ new CHapiXMLNode("Instance_" + std::to_string(currentInstanceNum)) };
			xmlEntityTypeNode->AddChild(xmlEntityInstanceNode);
			currentInstanceNum++;

			/*
			Create transform node
			*/
			CHapiXMLNode* xmlEntityTransformComponentNode{ new CHapiXMLNode("Transform") };
			std::shared_ptr<TransformComponent> currentEntityTransform{ ecsArg.GetComponent<TransformComponent>(entityID) };

			CHapiXMLAttribute xmlAttribute_XComponentOfTransform("x", currentEntityTransform->position.x);
			xmlEntityTransformComponentNode->AddAttribute(xmlAttribute_XComponentOfTransform);

			CHapiXMLAttribute xmlAttribute_YComponentOfTransform("y", currentEntityTransform->position.y);
			xmlEntityTransformComponentNode->AddAttribute(xmlAttribute_YComponentOfTransform);

			xmlEntityInstanceNode->AddChild(xmlEntityTransformComponentNode);

			/*
			Create render node
			*/
			CHapiXMLNode* xmlEntityRenderComponentNode{ new CHapiXMLNode("Render")};
			std::shared_ptr<RenderComponent> currentRenderComponent{ecsArg.GetComponent<RenderComponent>(entityID)};

			CHapiXMLAttribute xmlAttribute_renderLayerPrecedenceComponent("renderLayerPrecedence", currentRenderComponent->m_renderLayerPrecedence);
			xmlEntityRenderComponentNode->AddAttribute(xmlAttribute_renderLayerPrecedenceComponent);

			xmlEntityInstanceNode->AddChild(xmlEntityRenderComponentNode);
			
		}
	}

	CHapiXML xmlTreeFromRootNode(xmlRootNode);
	if (!xmlTreeFromRootNode.Save(xmlFileNameArg))
		return false;
	else
		return true;
}
bool Utils::Load_XML_Into_ECS(const std::string& xmlFileNameArg, ECS_Engine& ecsArg, ECS_Engine& prefabEcsArg, RenderManager& renderManager)
{
	//load the xml file
	CHapiXML xmlFile(xmlFileNameArg);
	
	//document pointer
	//if no root node exists then the file did not load
	CHapiXMLNode* rootNode = xmlFile.GetRootNode();
	
	if (!rootNode)
		return false;

	ecsArg.DeletePrefabInstansiatedEntities(prefabEcsArg);
	
	//entity node
	rootNode = rootNode->GetChildren()[0];

	//loop through all entiity types i.e. player, monster, alien....
	for (CHapiXMLNode* entityChild : rootNode->GetChildren())
	{
		//as its the prefab there will only be one ID
		const std::vector<uint32_t>& vectorOfIds{ prefabEcsArg.GetTaggedEntities(entityChild->GetName()) };

		if (vectorOfIds.size() == 0)
		{
			std::cerr << std::string("Entity Prefab :" + entityChild->GetName() + " failed to load from the provided prefab system");
			return false;
		}

		//get the id of the entity type from the prefab engine
		uint32_t entityTypeID_prefab{ prefabEcsArg.GetTaggedEntities(entityChild->GetName())[0] };

		//loop through instances of the entity type
		for (CHapiXMLNode* entityChildInstance : entityChild->GetChildren())
		{
			uint32_t createdEntityFromInstance{ecsArg.InstantiateEntity()};
			ecsArg.LoadEntityFromSystemIntoSystem(createdEntityFromInstance, entityTypeID_prefab, prefabEcsArg);
			


			//loop through all the components of the instance
			for (CHapiXMLNode* entityChildInstanceComponent : entityChildInstance->GetChildren())
			{
				if (entityChildInstanceComponent->GetName() == "Transform")
				{
					CHapiXMLAttribute xComponentForTransformAttribute;
					entityChildInstanceComponent->GetAttributeWithName("x", xComponentForTransformAttribute, false);
					
					CHapiXMLAttribute yComponentForTransformAttribute;
					entityChildInstanceComponent->GetAttributeWithName("y", yComponentForTransformAttribute, false);
					
					ecsArg.GetComponent<TransformComponent>(createdEntityFromInstance)->position = Vector2f(xComponentForTransformAttribute.AsFloat(), yComponentForTransformAttribute.AsFloat());
		
				}
				else if (entityChildInstanceComponent->GetName() == "Render")
				{
					CHapiXMLAttribute renderLayerPrecedenceAttribute;
					entityChildInstanceComponent->GetAttributeWithName("renderLayerPrecedence", renderLayerPrecedenceAttribute, false);

					ecsArg.GetComponent<RenderComponent>(createdEntityFromInstance)->m_renderLayerPrecedence = static_cast<int32_t>(renderLayerPrecedenceAttribute.AsInt());
				}
			}
			/*
			Need to add a component in the xml file that has attributes that tells me if I should tag the new entity in local ecs and also add
			it to the render layer.

			For now just add and tag anyway
			*/
			ecsArg.TagEntity(entityChild->GetName(), createdEntityFromInstance);
			renderManager.AddEntityIdToRenderLayer(ecsArg, createdEntityFromInstance, ecsArg.GetComponent<RenderComponent>(createdEntityFromInstance)->m_renderLayer);

		}

	}
	renderManager.RecalculateGenericRenderLayerPrecedenceCount(ecsArg);
	
	return true;

}
;

