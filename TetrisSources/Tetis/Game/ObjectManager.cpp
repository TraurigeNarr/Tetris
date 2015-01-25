#include "stdafx.h"

#include "ObjectManager.h"

#include "GameObject.h"
#include "IRenderer.h"

void ObjectManager::Refresh()
	{
	for (auto& pair : m_grouped_objects)
		{
		auto& vector(pair.second);

		vector.erase(
			std::remove_if(std::begin(vector), std::end(vector),
			[](GameObject* p_object){ return p_object->IsDestroyed(); }),
			std::end(vector));
		}

	m_objects.erase(
		std::remove_if(std::begin(m_objects), std::end(m_objects),
		[](const std::unique_ptr<GameObject>& p_unique_ptr){ return p_unique_ptr->IsDestroyed(); }),
		std::end(m_objects));
	}