#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>


#include <Optimus/Graphics/Core/SubRender.h>
#include <Optimus/Graphics/Models/QuadModel.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Graphics/Images/Image2D.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Application.h>
#include <Optimus/Log.h>

namespace OP
{
	class OPTIMUS_API Mesh
	{
	public:
		Mesh(const char* filename) :
			m_Quad(std::make_unique<QuadModel>()),
			m_Image(std::make_unique<Image2D>(filename)),
			m_UniformHandler(),
			m_DescriptorHandler()
		{
			OP_CORE_INFO("Mesh Created");
			m_UniformHandler.SetMode(0); //For static Meshes
		}

		virtual void Render(const CommandBuffer& commandBuffer, const Pipeline& pipeline)
		{
			if (!m_DescriptorHandler.Update(m_UniformHandler, *m_Image,  pipeline))
				return;

			//Updates the uniform
			m_UniformHandler.Update();

			//Bind the descriptors
			m_DescriptorHandler.BindDescriptor(commandBuffer);

			//Render the quad TODO: (Loop through all the meshes in the scene instead of one quad)
			m_Quad->CmdRender(commandBuffer);
		}

		//TODO: For testing purposes - PUT in Transform Component
		void PushTransform(glm::vec3 translate, float angle, glm::vec3 rotateaxis, glm::vec3 scale)
		{
			m_UniformHandler.GetWorldTransformMatrix() = glm::translate(glm::mat4(1.0f), translate);
			m_UniformHandler.GetWorldTransformMatrix() = glm::rotate(m_UniformHandler.GetWorldTransformMatrix(), angle, rotateaxis);
			m_UniformHandler.GetWorldTransformMatrix() = glm::scale(m_UniformHandler.GetWorldTransformMatrix(), scale);
		}

		virtual ~Mesh()
		{
			OP_CORE_INFO("Destroying Mesh");
		}

	protected: 
		std::unique_ptr<QuadModel> m_Quad; //Model
		std::unique_ptr <Image2D> m_Image; //Material

		UniformHandler m_UniformHandler;  //More like Transform Matrices Handler
		DescriptorHandler m_DescriptorHandler;

		//Transform Component stuff
		glm::vec3 m_Translate;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;
	};

}