#ifndef SCRIPTALLOCATOR_H
#define SCRIPTALLOCATOR_H

class BaseScript;

#include<vector>
#include<typeindex>
#include<unordered_map>
#include<memory>
#include<assert.h>

/*
The class compensates for a lack of a memory manager. We need to create various script instances on the heap
at run time but before the game loop as no not be bottlenecked by the OS.
*/

class ScriptAllocator final
{
public:
	//singleton method
	static ScriptAllocator& GetInstance();

	//Gets a script from the data store thats not in use
	template<class T>
	std::shared_ptr<T> GetScript();

	//Once a script is no longer needed its marked as open to use for another component wanting a script of the same type
	void FreeScript(const std::shared_ptr<BaseScript>& scriptToFree);

private:
	//Used to allocate scripts
	ScriptAllocator();
	~ScriptAllocator() = default;

	//a function that allocates N number of scripts of a given type so long as its base class is BaseScript
	template<class T>
	void AllocateScripts(size_t numberOfScripts);

	//The data store of types to allocated scripts
	std::unordered_map <std::type_index, std::vector<std::shared_ptr<BaseScript>>> m_componentTypeInstanceMap;
	

};

template<class T>
inline std::shared_ptr<T> ScriptAllocator::GetScript()
{
	//get type T typeID
	std::type_index scriptTypeID{typeid(T)};
	//if the type is not known throw break point
	assert(m_componentTypeInstanceMap.find(scriptTypeID) != m_componentTypeInstanceMap.end());

	//loop all known scripts of typeT
	for (const std::shared_ptr<BaseScript>& script : m_componentTypeInstanceMap[scriptTypeID])
	{
		//if the script is not being used by another script component
		if (!script->isScriptInUse)
		{
			//mark as in use by related caller of function
			script->isScriptInUse = true;
			//return script down casted to type T
			return std::static_pointer_cast<T>(script);
		}
	}
	//if no scripts find output to log, thow break point and return nullptr
	std::cerr << "No scripts avaliable!\n";
	assert(0);
	return nullptr;
}

template<class T>
inline void ScriptAllocator::AllocateScripts(size_t numberOfScripts)
{
	//get the script typeID
	std::type_index scriptType{ typeid(T) };
	//loop number of instances
	for (size_t scriptInstanceIndex{ 0 }; scriptInstanceIndex < numberOfScripts; scriptInstanceIndex++)
		m_componentTypeInstanceMap[scriptType].emplace_back(std::make_shared<T>());
}

#endif

