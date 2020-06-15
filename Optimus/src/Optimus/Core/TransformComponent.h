#pragma once

#include <Optimus/Core.h>
#include <Optimus/Core/Component.h>
#include <glm.hpp>

namespace OP
{
	class OPTIMUS_API TransformComponent : public Component
	{
	public:
		TransformComponent();
		~TransformComponent();

		void Serialize(JSONObject const& object);

		glm::vec2 GetTranslation()const { return m_Translation; }
		void SetTranslation(glm::vec2 translate) { m_Translation = translate; }

		double GetRotation()const { return m_Rotation; }
		void SetRotation(double rotation) { m_Rotation = rotation; }

		glm::vec2 GetScale()const { return m_Scale; }
		void SetScale(glm::vec2 scale) { m_Scale = scale; }


	private:
		glm::vec2	m_Translation;
		double		m_Rotation;
		glm::vec2	m_Scale;
	};
}

