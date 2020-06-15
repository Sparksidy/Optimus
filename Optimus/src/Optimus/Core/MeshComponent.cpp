#include <pch.h>
#include <Optimus/Core/MeshComponent.h>

#include <Optimus/Log.h>

namespace OP
{
	MeshComponent::MeshComponent():
		Component(MESH_COMPONENT),
		m_Quad(std::make_unique<QuadModel>()),
		m_UniformHandler(),
		m_DescriptorHandler()
	{
		OP_CORE_INFO("Mesh Created");
		m_UniformHandler.SetMode(0); //For static Meshes
	}

	MeshComponent::~MeshComponent()
	{
		OP_CORE_INFO("Destroying Mesh");
	}

	void MeshComponent::Serialize(JSONObject const& object)
	{
		auto it = object.FindMember("Texture");
		if (it != object.MemberEnd())
		{
			//Create the image
			std::string str = it->value.GetString();
			m_Image = std::make_unique<Image2D>(it->value.GetString());
		}
	}

	void MeshComponent::Render(const CommandBuffer& commandBuffer, const Pipeline& pipeline)
	{
		if (!m_DescriptorHandler.Update(m_UniformHandler, *m_Image, pipeline))
			return;

		//Updates the uniform
		m_UniformHandler.Update();

		//Bind the descriptors
		m_DescriptorHandler.BindDescriptor(commandBuffer);

		//Render the quad TODO: (Loop through all the meshes in the scene instead of one quad)
		m_Quad->CmdRender(commandBuffer);
	}
	void MeshComponent::PushTransform(glm::vec2 translate, float angle, glm::vec2 scale)
	{
		m_UniformHandler.GetWorldTransformMatrix() = glm::translate(glm::mat4(1.0f), glm::vec3(translate, 0.0f));
		m_UniformHandler.GetWorldTransformMatrix() = glm::rotate(m_UniformHandler.GetWorldTransformMatrix(), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		m_UniformHandler.GetWorldTransformMatrix() = glm::scale(m_UniformHandler.GetWorldTransformMatrix(), glm::vec3(scale, 1.0f));	
	}
}