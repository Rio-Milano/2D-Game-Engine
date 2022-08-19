#ifndef UTILS_H
#define UTILS_H

class ECS_Engine;
class RenderManager;
class UI_Manager;
enum class SaveSelection;

#include<string>

namespace Utils
{

	template <class T>
	inline T ClampValue(const T& toClamp, const T& leftClamp, const T& rightClamp)
	{
		if (toClamp < leftClamp)
			return leftClamp;
		else if (toClamp > rightClamp)
			return rightClamp;
		else
			return toClamp;
	};

	template<class T>
	T Max(const T& A, const T& B)
	{
		if (A > B)
			return A;
		else
			return B;
	}

	void LoadLevel
	(
		bool& saveLoaded,
		std::string& fileName,
		const SaveSelection& saveSelection,
		ECS_Engine& localEcs,
		ECS_Engine& prefabEcs,
		RenderManager& renderManager,
		UI_Manager& uiManager
	);

	/*
	The data model I am using to store world states is very simple. All entities are premade by the programmer,
	then added to a list box in the editor where entities can be placed into the scene. I then only take into account the new position and tag name
	when saving as everything else stays the same.

	Example XML Tree:

		entities
			|
		=========
		|       |
	  Zombie  Player
	  |  |  |  |.....
	  Instance 1, 2, 3...
	  |
	  Transform x, y
	  +others if needed

      When the editor becomes more complex i.e. has more UI features to create custom entities at run time. 
	  The data model must change to still work. A solution to this would be having a child node for every 
	  component of every arbitrary entity that has components. i.e.

	  <Entities>
		  <Entity_1>
				<TransformComponent x = 1 y = 1/>
				<TextComponent style = someEnumElement, size = n, ...>
					the text of the text component					
				</Text>
				..more components with data and attributes
		  </Entity_1>
		  
		  <Entity_2>
		  ...
		  ...
		  ..
	  </Entities>

	  We would also need to find a way to link to a scripting language such as python and provide adequate support for entity scripting Finally,
	  we would have to add 2 more pure virtual function to the Base Component class. One for making an xml node of itself 
	  and one for taking an xml node and using its attributes to initialize itself;

	  This would mean when saving state data we would go through each and every entity and make a node for each. Each node would have children 
	  (entity components) made up from attached components through the use of the first aforementioned function.

      When loading we would loop through each entity node then loop on children of entity. Then use the tag name to create the appropriate component of each child. 
	  Then use the 2nd aforementioned function to use the appropriate child to initialize the component. 
	  
	  To conclude: My current model allows for some expansion but will surely not be suitable for a dynamic system.

	*/

	bool SaveEcsToXML(const std::string& xmlFileNameArg, ECS_Engine& ecsArg, ECS_Engine& prefabEcsArg);
	bool Load_XML_Into_ECS(const std::string& xmlFileNameArg, ECS_Engine& ecsArg, ECS_Engine& prefabEcsArg, RenderManager& renderManager);
	
};

#endif