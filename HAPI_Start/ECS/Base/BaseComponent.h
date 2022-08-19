#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H


struct BaseComponent 
{
	BaseComponent() = default;
	~BaseComponent() = default;

	//A flag to "un-allocate" pre-allocated memory after its no longer in use.
	bool isInUse{ false };
	bool attributesExternalLinkage{ false };
};

#endif