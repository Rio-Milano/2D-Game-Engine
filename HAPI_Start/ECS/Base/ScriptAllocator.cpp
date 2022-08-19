#include "ScriptAllocator.h"

#include"../ECS/Base/BasesScript.h"
#include"../ECS/MyScripts/AllScriptHeadders.h"

void ScriptAllocator::FreeScript(const std::shared_ptr<BaseScript>& scriptToFree)
{
    scriptToFree->isScriptInUse = false;
}

ScriptAllocator::ScriptAllocator()
{
    //Initialize scripts here
    AllocateScripts<AlphaThingScript>(50);
    AllocateScripts<CameraScript>(10);
    AllocateScripts<PlayerScript>(1);
    AllocateScripts<TextElementInteractionResponseScript>(30);
    AllocateScripts<ListBoxWidgetSlideBarScript>(1);
    AllocateScripts<ButtonControllerScript>(30);
    AllocateScripts<BulletScript>(500);
    AllocateScripts<WorldSpawnerScript>(40);
    AllocateScripts<SilverKnightScript>(30);
    AllocateScripts<BommerScript>(30);
}

//Singleton method
ScriptAllocator& ScriptAllocator::GetInstance()
{
    static ScriptAllocator instance;
    return instance;
}
