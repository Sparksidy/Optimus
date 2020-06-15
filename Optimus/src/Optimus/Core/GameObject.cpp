#include <pch.h>
#include <Optimus/Core/GameObject.h>
#include <Optimus/Utilities/Macros.h>
#include <Optimus/Core/MeshComponent.h>
#include <Optimus/Core/TransformComponent.h>
#include <Optimus/Core/Component.h>


namespace OP
{
	GameObject::GameObject():m_IsActive(true)
	{
		for (int i = 0; i < NUM_OF_COMPONENTS; i++)
			m_Components[i] = nullptr;
	}

	GameObject::~GameObject()
	{
		for (int i = 0; i < NUM_OF_COMPONENTS; i++)
			SafeDelete(m_Components[i]);
	}

	Component* GameObject::GetComponent(COMPONENT_TYPE type)
	{
		return m_Components[type];
	}

	void GameObject::AddComponent(unsigned int mType, JSONObject const& object)
	{
		Component* newComponent = nullptr;
		switch (mType)
		{
		case TRANSFORM_COMPONENT:
			newComponent = new TransformComponent();
			break;
		case MESH_COMPONENT:
			newComponent = new MeshComponent();
			break;
		}

		if (newComponent)
		{
			newComponent->SetOwner(this);
			m_Components[mType] = newComponent;
			newComponent->Serialize(object);
		}
	}

}