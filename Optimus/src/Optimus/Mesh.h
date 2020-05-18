#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>


#include <Optimus/Graphics/Core/SubRender.h>
#include <Optimus/Graphics/Models/QuadModel.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Graphics/Images/Image2D.h>

#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Log.h>

namespace OP
{
	class OPTIMUS_API Mesh
	{
	public:
		Mesh(const char* filename, const GraphicsPipeline& pipeline) :
			m_Quad(std::make_unique<QuadModel>()),
			m_UniformHandler(),
			m_Image(std::make_unique<Image2D>(filename)),
			m_DescriptorHandler(*m_Image, m_UniformHandler, pipeline) 
		{
			OP_CORE_INFO("Mesh Created");
		}
		void Render(const CommandBuffer& commandBuffer)
		{
			//Updates the uniform
			m_UniformHandler.Update();

			//Bind the descriptors
			m_DescriptorHandler.BindDescriptor(commandBuffer);

			//Render the quad TODO: (Loop through all the meshes in the scene instead of one quad)
			m_Quad->CmdRender(commandBuffer);
		}

		~Mesh()
		{
			OP_CORE_INFO("Destroying Mesh");
		}

	private: 
		std::unique_ptr<QuadModel> m_Quad; //Model
		UniformHandler m_UniformHandler;   //Transform
		std::unique_ptr <Image2D> m_Image; //Material

		DescriptorHandler m_DescriptorHandler; //Resource Manager
	};

}