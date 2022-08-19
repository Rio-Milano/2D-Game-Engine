#include"AnimationSystem.h"


#include"../ECS/Base/Core_ECS_Assets.h"

#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ECS_Engine.h"

#include<string>

AnimationSystem::AnimationSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<AnimationComponent>();
}

std::shared_ptr<BaseComponent> AnimationSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	std::shared_ptr<AnimationComponent> toClone{ std::static_pointer_cast<AnimationComponent>(m_componentEngine.GetComponent(entityID)) };
	if (!toClone)
	{
		std::cerr << "Animation Component of entity :" + std::to_string(entityID) + " was not found\n";
		return nullptr;
	}


	std::shared_ptr<AnimationComponent> duplicatedComponent = forignECS.AddComponent<AnimationComponent>(forignEntityID);

	duplicatedComponent->InitializeAnimationComponent(
			toClone->m_animation_name,
			toClone->m_animationFrame,
			toClone->speed,
			toClone->m_isActive,
			toClone->m_Looping,
			toClone->m_DestroyOnFinish
	);

	return duplicatedComponent;
}

void AnimationSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	//we must have an animation component
	std::shared_ptr<AnimationComponent> animationComponent = ecs.GetComponent<AnimationComponent>(entityID);

	//if animation name stored in animation component is not a key in the animation data store then report error
	if (m_animationDataStore.find(animationComponent->m_animation_name) == m_animationDataStore.end())
	{
		std::cerr << std::string("Animation :" + animationComponent->m_animation_name + " was not found\n");
		return;
	}
	//we may not have a sprite component
	std::shared_ptr<SpriteComponent> spriteComponent = ecs.GetComponent<SpriteComponent>(entityID);
	if (!spriteComponent)
	{
		std::cerr << "Sprite Component of entity :" + std::to_string(entityID) + " was not found\n";
		return;
	}

	//update frame
	if (animationComponent->m_isActive)
	{
		size_t animationSize{ m_animationDataStore[animationComponent->m_animation_name].size() };

		while (animationComponent->elapsed >= animationComponent->speed)
		{
			if(animationComponent->m_Looping)
				animationComponent->m_animationFrame = (static_cast<size_t>(animationComponent->m_animationFrame) + static_cast<size_t>(1)) % (animationSize);
			else
			{
				//if animation frame is smaller than the last animation key frame index
				if (animationComponent->m_animationFrame < animationSize - 1)
					animationComponent->m_animationFrame++;
				//if on the last frame
				else
				{
					//the animation is complete as not looping
					animationComponent->m_isActive = false;
					animationComponent->m_hasFinished = true;
					if (animationComponent->m_DestroyOnFinish)
						ecs.GetComponent<RenderComponent>(entityID)->m_isActive = false;
				}
			}
			animationComponent->elapsed -= animationComponent->speed;
		}
		animationComponent->elapsed += deltaTime;
	}
	//set frame
	spriteComponent->m_sprite.m_textureRef = m_animationDataStore[animationComponent->m_animation_name][animationComponent->m_animationFrame];
}


//public system functions

void AnimationSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
	//we may not have an animation component
	std::shared_ptr<AnimationComponent> animationComponent = ecs.GetComponent<AnimationComponent>(entityID);

	if (!animationComponent)
	{
		std::cerr << "Animation Component of entity :" + std::to_string(entityID) + " was not found\n";
		return;
	}
}



void AnimationSystem::AddAnimationFromSpriteSheet(
												const std::string& animationName,
												const std::shared_ptr<Texture>& spriteSheet,//this texture will have our spritesheet
												const int& FramesOnXaxis,//frames on x axis
												const int& FramesOnYaxis,//frames on y axis
												const int& startFrameXIndex,//start frame
												const int& startFrameYIndex,//end frame
												const int& frameCount//number of frames
)
{
	//if animation name is already in data store then report error
	if (m_animationDataStore.find(animationName) != m_animationDataStore.end())
	{
		std::cerr << std::string("Animation :" + animationName + " already exists");
		return;
	}


	//calculate the dimantion of each animation
	const int animationFrameWidth{ spriteSheet->m_textureSize.x / FramesOnXaxis };
	const int animationFrameHeight{ spriteSheet->m_textureSize.y / FramesOnYaxis };


	int createdFrames{ 0 };
	for (int y{ startFrameYIndex }; y < FramesOnYaxis; y++)
	{
		for (int x {startFrameXIndex}; x < FramesOnXaxis; x++)
		{
			if (createdFrames++ < frameCount)
			{
				//each frame
				std::shared_ptr<Texture> newAnimationFrame{ std::make_shared<Texture>() };

				//set animation dimentions
				newAnimationFrame->m_textureSize.x = animationFrameWidth;
				newAnimationFrame->m_textureSize.y = animationFrameHeight;

				//init the rect of the animation
				newAnimationFrame->m_textureRect.x1 = 0;
				newAnimationFrame->m_textureRect.y1 = 0;
				newAnimationFrame->m_textureRect.x2 = animationFrameWidth;
				newAnimationFrame->m_textureRect.y2 = animationFrameHeight;

				BYTE* derivedTextureBuffer { new BYTE[(static_cast<long long int>(animationFrameHeight) * static_cast<long long int>(animationFrameWidth))* (static_cast<long long int>(4))]};
				
				
				int offsetToDerivedTexture{ (x * animationFrameWidth + (y * animationFrameHeight)*spriteSheet->m_textureSize.x) * 4 };

				for (int derivedTextureHeight{ 0 }; derivedTextureHeight < animationFrameHeight; derivedTextureHeight++)
				{
					memcpy(
						derivedTextureBuffer + (static_cast<long long int>(derivedTextureHeight) * static_cast<long long int>(animationFrameWidth))* static_cast<long long int>(4),
						spriteSheet->m_textureBuffer + offsetToDerivedTexture + (static_cast<long long int>(derivedTextureHeight) * static_cast<long long int>(spriteSheet->m_textureSize.x))* static_cast<long long int>(4),
						static_cast<long long int>(animationFrameWidth)* static_cast<long long int>(4));
				}

				newAnimationFrame->m_textureBuffer = derivedTextureBuffer;

				//add animation name to data store
				m_animationDataStore[animationName].emplace_back(newAnimationFrame);

			}
			else
				return;
		}
	}


}


void AnimationSystem::AddAnimationFromTextures(
	const std::string& animationName, 
	const std::string& fileNamePrefix, 
	const std::string& fileExtension,
	const Vector2i& animationFrameSize, 
	const int& frameCount, 
	const size_t& bytesPerPixel)
{

	for (int frame{ 0 }; frame < frameCount; frame++)
	{
		std::shared_ptr<Texture> keyFrame{TextureManager.LoadTexture(fileNamePrefix + std::to_string(frame) + fileExtension, bytesPerPixel)};
		m_animationDataStore[animationName].emplace_back(keyFrame);
	}
}

std::shared_ptr<Texture> AnimationSystem::GetAnimationFrame(const std::string& animationName, const int& frame)
{
	//if animation name not key in data store
	if (m_animationDataStore.find(animationName) == m_animationDataStore.end())
	{
		std::cerr << std::string("Animation :" + animationName + " was not found!\n");
		return nullptr;
	}

	std::vector<std::shared_ptr<Texture>>& animationFrames{ m_animationDataStore[animationName] };

	if (animationFrames.size() > 0)
	{
		if (frame < 0 || frame >= animationFrames.size())
		{ 
			std::cerr << "You searched for key frame :" + std::to_string(frame) + " in animation :" << animationName << " which was out of range\n";
			return nullptr;
		}

		return animationFrames[frame];
	}
	else
	{
		std::cerr << "You searched for key frame :" + std::to_string(frame) + " in animation :" << animationName << " when there are no animations present\n";
		return nullptr;
	}
}

const std::vector<std::shared_ptr<Texture>>& AnimationSystem::GetAnimationBuffer(const std::string& animationBufferName)
{
	return m_animationDataStore[animationBufferName];
}

void AnimationSystem::LoadAnimationsToSystemFromSystem(ECS_Engine& forignEcsEngine)
{
	const std::shared_ptr<AnimationSystem> forignAnimationSystem{forignEcsEngine.GetSystem<AnimationSystem>()};
	if (!forignAnimationSystem)
	{
		std::cerr << "Forign animation system not found!";
		return;
	}

	for (const std::pair<std::string, std::vector<std::shared_ptr<Texture>>>& prefabAnimationRecord : forignAnimationSystem->m_animationDataStore)
		this->m_animationDataStore[prefabAnimationRecord.first] = prefabAnimationRecord.second;

}

void AnimationSystem::PlayAnimation(const uint32_t& entityID, const std::string& animationName, ECS_Engine& ecs)
{
	std::shared_ptr<AnimationComponent> animationComponent{ecs.GetComponent<AnimationComponent>(entityID)};
	if (!animationComponent)
	{
		std::cerr << std::string("Could not play the animation with the name " + animationName + " because the animation component was missing");
		return;
	}

	std::shared_ptr<RenderComponent> renderComponent{ ecs.GetComponent<RenderComponent>(entityID) };
	if (!animationComponent)
	{
		std::cerr << std::string("Could not play the animation with the name " + animationName + " because the render component was missing");
		return;
	}
	std::shared_ptr<SpriteComponent> spriteComponent{ ecs.GetComponent<SpriteComponent>(entityID) };
	if (!spriteComponent)
	{
		std::cerr << std::string("Could not play the animation with the name " + animationName + " because the sprite component was missing");
		return;
	}

	animationComponent->m_animation_name = animationName;
	animationComponent->m_isActive = true;
	animationComponent->m_hasFinished = false;
	animationComponent->m_animationFrame = 0;
	renderComponent->m_isActive = true;
	spriteComponent->m_sprite.m_textureRef = GetAnimationFrame(animationName, 0);

	
}
