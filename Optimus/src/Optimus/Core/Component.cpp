#include <pch.h>
#include <Optimus/Core/Component.h>

namespace OP
{
	Component::Component(unsigned int Type):m_ComponentType(Type), m_Owner(nullptr), m_IsActive(false)
	{
	}
}