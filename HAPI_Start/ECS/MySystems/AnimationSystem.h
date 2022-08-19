#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include"../base/system.h"
#include<utility>
#include"../GeneralObjects/Rect.h"
#include"../GeneralObjects/Texture.h"

class Renderer;
class ECS_Engine;

class AnimationSystem final : public System
{

public:
	AnimationSystem(const UpdatePriority& updatePriority);

	void ResetComponent(const uint32_t& entityid, ECS_Engine& ecs) override;


	void AddAnimationFromSpriteSheet(		
										const std::string& animationName,
										const std::shared_ptr<Texture>& texture,
										const int& animationsOnXaxis, 
										const int& animationsOnYaxis, 
										const int& startFrameX,
										const int& startFrameY, 
										const int& frameCount);

	void AddAnimationFromTextures(
		const std::string& animationName,
		const std::string& fileNamePrefix,
		const std::string& fileExtension,
		const Vector2i& animationFrameSize,
		const int& frameCount,
		const size_t& bytesPerPixel);

	std::shared_ptr<Texture> GetAnimationFrame(const std::string& animationName, const int& frame);

	const std::vector<std::shared_ptr<Texture>>& GetAnimationBuffer(const std::string& animationBufferName);

	void LoadAnimationsToSystemFromSystem(ECS_Engine& forignEcsEngine);
	
	void PlayAnimation(const uint32_t& entityID, const std::string& animationName, ECS_Engine& ecs);


private:
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)override;
	virtual void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime) override;

	//animationName ---> [textureName, vector[rect]]
	std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> m_animationDataStore;
};

#endif
