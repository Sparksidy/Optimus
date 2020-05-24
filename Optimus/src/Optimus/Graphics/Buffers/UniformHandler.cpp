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

		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		//Move the object to the center and spin
		m_UniformBlock.model =  glm::translate(glm::mat4(1.0f), glm::vec3((float)Application::Get().GetWindow().GetWindowWidth() / 2, (float)Application::Get().GetWindow().GetWindowHeight() / 2, 0.0f));
		m_UniformBlock.model = glm::rotate(m_UniformBlock.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_UniformBlock.model = glm::scale(m_UniformBlock.model,  glm::vec3(10.0f, 10.0f, 1.0f));


		m_UniformBlock.view = glm::mat4(1.0f);
		m_UniformBlock.projection = glm::ortho (0.f, (float)Application::Get().GetWindow().GetWindowWidth(), 0.f, (float)Application::Get().GetWindow().GetWindowHeight(), -1.f, 1.f);

		m_UniformBuffers[imageIndex]->Update(&m_UniformBlock);
	}
}