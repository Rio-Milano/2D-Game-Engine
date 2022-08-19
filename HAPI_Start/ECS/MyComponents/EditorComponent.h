#ifndef EDITORCOMPONENT_H
#define EDITORCOMPONENT_H

#include"../ECS/Base/BaseComponent.h"
#include"../DataStructures/Vector.h"

struct  EditorComponent final : public BaseComponent
{
	void InitializeEditorComponent(const bool& moveByMouseArgRef, const bool& spatialLockArgRef = true)
	{
		m_enableMoveByMouse = moveByMouseArgRef; 
		m_spatialLock = spatialLockArgRef;

	}

	bool
		m_enableMoveByMouse{ true },
		m_hasClickPointOffsetBeenSet{ false },
		m_focus{ false };

	Vector2i m_clickPointOffset{ 0,0 };

	bool m_spatialLock{},
		m_oneInstanceOnly{false};


};

#endif