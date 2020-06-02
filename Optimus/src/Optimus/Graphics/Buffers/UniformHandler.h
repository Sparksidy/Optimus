#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>


#include <Optimus/Utilities/NonCopyable.h>
#include <Optimus/Graphics/Buffers/UniformBuffer.h>
#include <Optimus/Graphics/Descriptors/DescriptorHandler.h>
#include <glm.hpp>

namespace OP
{
	//TODO: Make this dynamic so that we can add to uniform blocks
	struct UniformBlock
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		//Test: ColorMode(0), TextureMode(1), AnimatedMode(2)
		int mode;
		glm::mat3 textureMatrix;
	};


	class OPTIMUS_API UniformHandler : public NonCopyable
	{
	public:
		explicit UniformHandler();

		void Update();

		UniformBuffer* GetUniformBuffer(uint32_t imageIndex) { return m_UniformBuffers[imageIndex].get(); }

		void SetTextureMatrix(glm::mat3& tMatrix) { m_UniformBlock.textureMatrix = tMatrix; }

		void SetMode(int mode) { m_UniformBlock.mode = mode; }

		const size_t GetSizeOfUniformBlock()const { return sizeof(m_UniformBlock); }

		glm::mat4& GetWorldTransformMatrix(){ return m_UniformBlock.model; }

	private:
		UniformBlock m_UniformBlock;
		uint32_t m_Size = 0;
		std::vector<std::unique_ptr<UniformBuffer>> m_UniformBuffers; //uniform buffer per swapchain image
	};
}