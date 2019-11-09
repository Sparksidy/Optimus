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
	class CommandPool;

	class OPTIMUS_API Graphics
	{
	public:
		Graphics();

		~Graphics();

		void Init();

		void Update();


		const LogicalDevice* GetLogicalDevice() const { return m_LogicalDevice.get(); }

	private:
		void createCommandBuffers();
		void createGraphicsPipeline();
		void createSyncObjects();
		void recreateSwapchain();
		void cleanupSwapChain();

		VkShaderModule Graphics::createShaderModule(const std::vector<char>& code);

		void drawFrame();

		//TODO: Make this static and move it 
		std::vector<char> Graphics::readFile(const std::string& filename);


		std::unique_ptr<Instance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<SwapChain> m_SwapChain;
		std::unique_ptr<RenderPass> m_Renderpass;
		std::unique_ptr<Framebuffers> m_Framebuffers;
		std::unique_ptr<CommandPool> m_CommandPool;

		VkPipelineLayout m_PipelineLayout = {};
		VkPipeline m_GraphicsPipeline = {};
		std::vector<VkCommandBuffer> m_CommandBuffers = {};

		std::vector<VkSemaphore> m_ImageAvailableSemaphore;
		std::vector<VkSemaphore> m_RenderFinishedSemaphore;
		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;

		const int MAX_FRAMES_IN_FLIGHT = 2;
		size_t m_CurrentFrame = 0;
		bool recreatingSwapchain = false;
		bool framebufferResized = false;
	};
}