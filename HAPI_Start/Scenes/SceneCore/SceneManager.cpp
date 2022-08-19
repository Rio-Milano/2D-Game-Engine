#include "SceneManager.h"

//include scene headders
#include"../MyScenes/AllSceneHeadders.h"

#include<iostream>


void SceneManager::LoadScenes(Renderer& renderer)
{
	//load all of the scenes
	AddScene<MainScene>("MainScene", Scene_Enum::MainScene, renderer);
	AddScene<StartScene>("StartScene", Scene_Enum::StartScene, renderer);
	AddScene<MainMenu>("MainMenu", Scene_Enum::MainMenuScene, renderer);
	AddScene<SaveSelectionScene>("SaveSelectionScene", Scene_Enum::SaveSelectionScene, renderer);
	AddScene<LevelEditorScene>("LevelEditorScene", Scene_Enum::SaveSelectionScene, renderer);
	AddScene<ConfigureScene>("ConfigureScene", Scene_Enum::ConfigureScene, renderer);
	AddScene<GameCycling>("GameCycling", Scene_Enum::GameCycling, renderer);
}



//load a scene onto the stack
void  SceneManager::LoadScene(const std::string& sceneName)
{
	//if the scene is not found
	if (m_scenes.find(sceneName) == m_scenes.end())
	{
		std::cerr << std::string("Scene :" + sceneName + " can not be loaded");
		return;
	}
	//add the scene object to the stack
	m_listOfVisualOverlays.emplace_back(m_scenes[sceneName]);

}
const bool SceneManager::IsSceneActive(const std::string& sceneName)
{
	//if the scene is not found
	if (m_scenes.find(sceneName) == m_scenes.end())
	{
		std::cerr << std::string("Scene :" + sceneName + " can not be found!\n");
		return false;
	}

	//get the scene from the heap to check
	std::shared_ptr<Scene> sceneQuery = m_scenes[sceneName];
	
	//would normally return false as not found but this is handier
	if (!sceneQuery)
	{
		std::cerr << std::string("Scene :" + sceneName + " can not be loaded");
		return false;
	}

	//loop through all of the scenes 
	for (const std::shared_ptr<Scene>& scene : m_listOfVisualOverlays)
	{
		// if the scene is in the heap then return true for found scene
		if (sceneQuery == scene)
			return true;
	}
	//return false for not found the scene
	return false;
}

//update the scene
void  SceneManager::UpdateScenes(const float& deltaTime)
{
	//loop through all the scenes
	for (const std::shared_ptr<Scene>& scene : m_listOfVisualOverlays)
		//update the scene
		scene->BaseUpdate(deltaTime, *this);
}

void SceneManager::RenderScenes(Renderer& renderer, const HAPI_TColour& clearColor, const bool& useRGBA)
{
	//clear screen
	renderer.ClearScreenColor();

	//loop through all the scenes
	for (const std::shared_ptr<Scene>& scene : m_listOfVisualOverlays)
		//render the scene
		scene->Render(renderer);

}

void  SceneManager::PopScene()
{
	//take the scene from the top of the stack and unload it
	m_listOfVisualOverlays[m_listOfVisualOverlays.size() - 1]->DeInit();

	//remove the scene from the top of the stack
	m_listOfVisualOverlays.pop_back();

};