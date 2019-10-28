#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>


namespace OP
{
	class Instance;
	class PhysicalDevice;
	class Surface;
	class LogicalDevice;
	class SwapChain;
	class RenderPass;
	class Framebuffers;

	class OPTIMUS_API Graphics
	{
	public:
		Graphics();

		~Graphics();

		void Update();

	private:

		void createGraphicsPipeline();
		void createCommandPool();
		void createCommandBuffers();

		//TODO: Make this static and move it 
		std::vector<char> Graphics::readFile(const std::string& filename);
		

		VkShaderModule Graphics::createShaderModule(const std::vector<char>& code);

		std::unique_ptr<Instance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<SwapChain> m_SwapChain;
		std::unique_ptr<RenderPass> m_Renderpass;

		std::unique_ptr<Framebuffers> m_Framebuffers;

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
}