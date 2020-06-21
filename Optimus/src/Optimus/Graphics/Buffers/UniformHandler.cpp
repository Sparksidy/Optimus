#include <pch.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Core/Graphics.h>
#include <Optimus/Window.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace OP
{
	UniformHandler::UniformHandler():
		m_Size(sizeof(m_UniformBlock))
	{
		size_t swapchainImages = 3;//GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();
		m_UniformBuffers.resize(swapchainImages);

		//Create a uniform buffer per swapchain image
		for (size_t i = 0; i < swapchainImages; i++)
		{
			m_UniformBuffers[i] = std::make_unique<UniformBuffer>(m_Size);
			OP_CORE_INFO("Uniform Buffer is created in the handler");   
		}
	}

	UniformHandler::~UniformHandler()
	{
		OP_CORE_INFO("Destroying Uniform Handler");
	}

	void UniformHandler::Update()  
	{
		uint32_t imageIndex = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetActiveImageIndex();

		m_UniformBlock.model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0,0,0));

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		
		m_UniformBlock.view = glm::lookAt(glm::vec3(0.0f, 0.0, -2.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));

		//m_UniformBlock.view =  glm::lookAt(glm::vec3(2.0f, 2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_UniformBlock.projection = glm::perspective(glm::radians(70.0f), (float)Application::Get().GetWindow().GetWindowWidth() / (float)Application::Get().GetWindow().GetWindowHeight(), 1.0f, 256.0f);
		m_UniformBlock.projection[1][1] *= -1;
		//m_UniformBlock.projection = glm::ortho(0.f, (float)Application::Get().GetWindow().GetWindowWidth(), 0.f, (float)Application::Get().GetWindow().GetWindowHeight() , -1.f, 1.f);
		
		m_UniformBuffers[imageIndex]->Update(&m_UniformBlock);
	}
}