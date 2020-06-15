#pragma once

#include <Optimus/Core.h>
#include <document.h>

namespace OP
{
	class GameObject;
	enum COMPONENT_TYPE : unsigned int
	{
		//Core
		TRANSFORM_COMPONENT,

		//Graphics
		MESH_COMPONENT,
		ANIMATED_MESH_COMPONENT,

		NUM_OF_COMPONENTS
	};

	class OPTIMUS_API Component
	{
	public:

		typedef rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<>>> JSONObject;

		Component(unsigned int Type);
		virtual ~Component() {};

		virtual void Serialize(JSONObject const& object) {};

		//Getters
		GameObject* GetOwner() { return m_Owner; }
		unsigned int GetComponentType()const { return m_ComponentType; }
		bool isActive()const { return m_IsActive; }

		//Setters
		void SetOwner(GameObject* go) { m_Owner = go; }
		void SetActive(bool value) { m_IsActive = value; }

	protected:
		GameObject*			m_Owner;
		unsigned int		m_ComponentType;
		bool				m_IsActive; //When creating components they are set to false by default, 
										//they are active when they are added to GO
	};
}