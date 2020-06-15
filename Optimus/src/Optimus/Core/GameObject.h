#pragma once

#include <Optimus/Core.h>
#include <Optimus/Core/Component.h>
#include <document.h>

namespace OP
{
	class OPTIMUS_API GameObject
	{
	public:

		friend class GameObjectManager;
		typedef rapidjson::GenericValue<rapidjson::UTF8<>>::Object JSONObject;

		GameObject();
		~GameObject();

		Component* GetComponent(COMPONENT_TYPE type);

	private:
		void AddComponent(unsigned int mType, JSONObject const& object);

		Component* m_Components[NUM_OF_COMPONENTS];
		bool m_IsActive;

		//TODO: Add layer number to the GameObject
	};
}