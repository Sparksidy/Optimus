#include <pch.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Core/Graphics.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>

namespace OP
{
	UniformHandler::UniformHandler():
		m_Size(sizeof(m_UniformBlock))
	{
		size_t swapchainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();
		m_UniformBuffers.resize(swapchainImages);

		//Create a uniform buffer per swapchain image
		for (size_t i = 0; i < swapchainImages; i++)
		{
			m_UniformBuffers[i] = std::make_unique<UniformBuffer>(m_Size);
		}
	}
	void UniformHandler::Update()
	{
		uint32_t imageIndex = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetActiveImageIndex();

        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        m_UniformBlock.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        m_UniformBlock.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        m_UniformBlock.projection = glm::perspective(glm::radians(90.0f), (float)GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().width/(float)GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().height, 0.1f, 10.0f);
        m_UniformBlock.projection[1][1] *= -1;


		m_UniformBuffers[imageIndex]->Update(&m_UniformBlock);
	}
}