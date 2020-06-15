#include <pch.h>
#include <Optimus/Core/TransformComponent.h>

namespace OP
{
	TransformComponent::TransformComponent() :
		Component(TRANSFORM_COMPONENT)
	{
	}
	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::Serialize(JSONObject const& object)
	{
		auto it = object.FindMember("Translation");
		if (it != object.MemberEnd())
		{
			auto translationArray = it->value.GetArray();
			m_Translation.x = translationArray[0].GetFloat();
			m_Translation.y = translationArray[1].GetFloat();
		}

		it = object.FindMember("Rotation");
		if (it != object.MemberEnd())
			m_Rotation = it->value.GetFloat();

		it = object.FindMember("Scale");
		if (it != object.MemberEnd())
		{
			auto scaleArray = it->value.GetArray();
			m_Scale.x = scaleArray[0].GetFloat();
			m_Scale.y = scaleArray[1].GetFloat();
		}
	}
}