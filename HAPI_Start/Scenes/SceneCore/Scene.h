#ifndef SCENE_H
#define SCENE_H

#include"Scene_Enum.h"

#include"../Managers/RenderManager.h"
#include"../Core/Renderer.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../UI/UI Core/UI_Manager.h"

/*
this is a base class that all other scenes can implement and then they are handeled by the scene manager
*/

class Renderer;
class SceneManager;

class Scene
{
public:
	friend class Visualisation;
	
	Scene(Renderer& renderer, const Scene_Enum& sceneEnum);
	~Scene() = default;

	//create a camera in the local ecs
	void InitCamera();

	void RefreshReloadedScene();

	//camera control
	void ControlCamera(const float& deltaTime);

	Vector2i GetCameraOffset();

	//will draw any components to the window
	virtual void Render(Renderer& renderer) = 0;

	void BaseUpdate(const float& deltaTime, SceneManager& sceneManager);

	//loads any content the scene needs
	virtual void Init() = 0;


	//unloads any content that the scene needs 
	virtual void DeInit() = 0;

	const Scene_Enum GetSceneEnum() const { return m_sceneEnum; };

	static bool  m_editorMode;

protected:
	Renderer& m_rendererReferance;

	RenderManager m_renderManager;
	UI_Manager m_UI_Manager;
	ECS_Engine m_ecsEngine;

	Vector2i m_previousMousePosition{0, 0};

	static ECS_Engine m_prefabEngine;

	bool m_sceneClosed{ true };

private:
	virtual void ProcessUI(SceneManager& sceneManager) {};
	//will update any components on the window
	virtual void Update(const float& deltaTime, SceneManager& sceneManager) = 0;

	Scene_Enum m_sceneEnum;

	
};

#endif