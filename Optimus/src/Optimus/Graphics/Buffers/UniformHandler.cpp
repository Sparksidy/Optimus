#include <pch.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Core/Graphics.h>
#include <Optimus/Window.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

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

		m_UniformBlock.view = glm::mat4(1.0f);
		m_UniformBlock.projection = glm::ortho(0.f, (float)Application::Get().GetWindow().GetWindowWidth(), 0.f, (float)Application::Get().GetWindow().GetWindowHeight(), -1.f, 1.f);
		
		m_UniformBuffers[imageIndex]->Update(&m_UniformBlock);
	}
}