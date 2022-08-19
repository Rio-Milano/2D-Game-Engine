#ifndef  SceneManager_H
#define SceneManager_H

#include <vector>
#include <unordered_map>
#include<string>
#include<memory>
#include"Scene.h"




class SceneManager
{
public:
	//ctors and dtors
	SceneManager() = default;
	~SceneManager() = default;

	void LoadScenes(Renderer& renderer);

	//get if a scene is on the stack display
	const bool IsSceneActive(const std::string& sceneName);

	//load a scene onto the stack
	void LoadScene(const std::string& sceneName);

	/*
	Utility Functions
	*/

	//update the scene
	void UpdateScenes(const float& deltaTime);

	//draw scene components onto the window
	void RenderScenes(Renderer& renderer, const HAPI_TColour& clearColor, const bool& useRGBA);

	//take the top scene from the stack and discard it
	void PopScene();

	//peak what scene is on top of the stack
	inline std::shared_ptr<Scene> PeakScene()const { return this->m_listOfVisualOverlays[m_listOfVisualOverlays.size() - 1]; };


private:
	
	template<class T>
	void AddScene(const std::string& sceneName, const Scene_Enum& sceneEnum, Renderer& renderer);

	//a stack of all scenes (not used std::stack as was unaware tit existed at the time)
	std::vector<std::shared_ptr<Scene>> m_listOfVisualOverlays;

	//a hash map mapping names of scenes to the location they are on the heap
	std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;

};

#endif 

template<class T>
inline void SceneManager::AddScene(const std::string& sceneName, const Scene_Enum& sceneEnum, Renderer& renderer)
{
	//check if scene exists
	if (m_scenes.find(sceneName) != m_scenes.end())
	{
		//if scene exists then report error
		std::cerr << std::string("Scene :" + sceneName + " already exists!\n");
		return;
	}
	
	//if scene does not exist then create scene using type and passed sceneEnum
	m_scenes[sceneName] = std::make_shared<T>(renderer, sceneEnum);
	
	//init the scene object
	m_scenes[sceneName]->Init();
}
