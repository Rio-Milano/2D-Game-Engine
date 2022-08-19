#ifndef VISUALISATION_H
#define VISUALISATION_H

#include"Renderer.h"
#include"../Scenes/SceneCore/SceneManager.h"

class BaseScript;
struct ScriptComponent;
struct MovementComponent;

class Visualisation
{
public:
	Visualisation();
	~Visualisation() = default;

	void Run();

	static constexpr float	deltaTime{ 1.0f / 60.0f };

private:
	uint32_t CreatePrefabEntity(const unsigned int& renderLayer,
		const std::shared_ptr<Texture>& texture,
		const bool& useAlphaChannels,
		const Effects& effects,
		const bool& moveByMouse,
		const std::string& tagName,
		const bool& spatialLock = true,
		const std::shared_ptr<BaseScript>& baseScript = nullptr,
		const bool& movementComponent = false,
		const bool& addCollisionComponent = false,
		const unsigned int& collisionLayer = 0,
		FRect collisionRect = FRect(0.0f, 0.0f, 0.0f, 0.0f)
	);

	Renderer m_renderer;

	bool m_hapiSuccess{ false };

	double
		elapsedTime{ 0 },
		previousTime{ 0 },
		currentTime{ 0 };

	Vector2i windowSize{ 1600, 900 };




	SceneManager m_sceneManager;

};

#endif