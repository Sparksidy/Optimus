#pragma once

#include <Optimus/Core.h>
#include <Optimus/Core/Component.h>
#include <glm.hpp>

#include <Optimus/Graphics/Models/QuadModel.h>
#include <Optimus/Graphics/Images/Image2D.h>

#include <Optimus/Graphics/Buffers/UniformHandler.h>

namespace OP
{
	//class DescriptorHandler;
	class OPTIMUS_API MeshComponent : public Component
	{
	public:
		MeshComponent();
		~MeshComponent();

		void Serialize(JSONObject const& object);
		
		//virtual as AnimatedMesh overrides this
		virtual void Render(const CommandBuffer& commandBuffer, const Pipeline& pipeline);

		//TODO: Looks like a hack
		void PushTransform(glm::vec2 translate, float angle, glm::vec2 scale);

	protected:
		std::unique_ptr<QuadModel> m_Quad; //Model
		std::unique_ptr <Image2D> m_Image; //Material

		UniformHandler m_UniformHandler;  //More like Transform Matrices Handler
		DescriptorHandler m_DescriptorHandler; //The Actual vulkan resource for the mesh

		//TODO: Transform Component stuff
		glm::vec3 m_Translate;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;
	};
}

