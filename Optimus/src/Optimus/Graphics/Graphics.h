#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>
#include <Optimus/ISystem.h>

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
	class GraphicsPipeline;
	class CommandBuffer;
	class Buffer;
	class DescriptorSetLayout;
	class DescriptorPool;
	class DescriptorSet;

	class OPTIMUS_API Graphics : public ISystem
	{
	public:
		Graphics();

		~Graphics();

		bool Initialize() override;
		void Update() override;
		void Unload() override {} //TODO

		inline std::string GetName()const { return "Graphics"; }

		//Getters
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice.get(); }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice.get(); }
		const SwapChain& GetSwapchain()const { return *m_SwapChain.get(); }
		const RenderPass& GetRenderPass()const { return *m_Renderpass.get(); }
		const Framebuffers& GetFramebuffers()const { return *m_Framebuffers.get(); }
		const CommandPool& GetCommandPool()const { return *m_CommandPool.get(); }
		const GraphicsPipeline& GetGraphicsPipeline()const { return *m_GraphicsPipeline.get(); }
		const Buffer& GetBuffer()const { return *m_Buffer.get(); }
		const DescriptorSetLayout& GetDescriptorSetLayout()const { return *m_DescriptorSetLayout.get(); }
		const DescriptorPool& GetDescriptorPool()const { return *m_DescriptorPool.get(); }
		const DescriptorSet& GetDescriptorSet()const { return *m_DescriptorSets.get(); }


	private:
		void createSyncObjects();
		void recreateSwapchain();
		void cleanupSwapChain();

		void drawFrame();

		std::unique_ptr<Instance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;

		std::unique_ptr<SwapChain> m_SwapChain;
		std::unique_ptr<RenderPass> m_Renderpass;
		std::unique_ptr<Framebuffers> m_Framebuffers;
		std::unique_ptr<CommandPool> m_CommandPool;
		std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline;
		std::unique_ptr<CommandBuffer> m_CommandBuffers;
		std::unique_ptr<Buffer> m_Buffer;
		std::unique_ptr<DescriptorSetLayout> m_DescriptorSetLayout;
		std::unique_ptr<DescriptorPool> m_DescriptorPool;
		std::unique_ptr<DescriptorSet> m_DescriptorSets;

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