#include "TextSystem.h"
#include"../ECS/MyComponents/TextComponent.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ECS_Engine.h"

TextSystem::TextSystem(const UpdatePriority& updatePriority)
    :System(updatePriority)
{
    m_componentEngine.AllocateComponents<TextComponent>();
}

void TextSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
    std::shared_ptr<TextComponent> textComponent{std::static_pointer_cast<TextComponent>(this->m_componentEngine.GetComponent(entityID))};
    textComponent->m_fillColour = HAPI_TColour::WHITE;
    textComponent->m_styleFlags = HAPI_TextStyle::eRegular;
    textComponent->m_text = "";
    textComponent->m_textSize = 30U;
}

std::shared_ptr<BaseComponent> TextSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
    std::shared_ptr<TextComponent> ToClone{ std::static_pointer_cast<TextComponent>(this->m_componentEngine.GetComponent(entityID)) };

    std::shared_ptr<TextComponent> duplicatedComponent = forignECS.AddComponent<TextComponent>(forignEntityID);

    duplicatedComponent->InitializeTextComponent(
        ToClone->m_fillColour, ToClone->m_text, ToClone->m_textSize, ToClone->m_styleFlags
    );

    return duplicatedComponent;

}
