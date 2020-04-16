#pragma once
#include <pch.h>
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
	class CommandPool;
	class GraphicsPipeline;
	class CommandBuffer;
	class Buffer;
	class DescriptorSetLayout;
	class DescriptorPool;
	class DescriptorSet;
	class Renderer;
	class QuadModel;

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
		const Instance& GetInstance() const { return *m_Instance.get(); }
		const Surface& GetSurface() const { return *m_Surface.get(); }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice.get(); }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice.get(); }
		const SwapChain& GetSwapchain()const { return *m_SwapChain.get(); }
		const std::shared_ptr<CommandPool>& GetCommandPool(const std::thread::id& threadId = std::this_thread::get_id());
		
		const GraphicsPipeline& GetGraphicsPipeline()const { return *m_GraphicsPipeline.get(); }
		const Buffer& GetBuffer()const { return *m_Buffer.get(); }
		const DescriptorSetLayout& GetDescriptorSetLayout()const { return *m_DescriptorSetLayout.get(); }
		const DescriptorPool& GetDescriptorPool()const { return *m_DescriptorPool.get(); }
		const DescriptorSet& GetDescriptorSet()const { return *m_DescriptorSets.get(); }
		const size_t& GetImageIndex()const { return m_ImageIndex; }
		const bool SwapchainRebuild() { return recreatingSwapchain;  }


		void SetRecreateSwapchain(bool result) { recreatingSwapchain = result; }

		Renderer* GetRenderer()const { return m_Renderer.get(); }
		void SetRenderer(std::unique_ptr<Renderer>&& renderer) { m_Renderer = std::move(renderer); }\
		RenderStage* GetRenderStage(uint32_t index);

	private:
		void resetRenderStages();
		void createSyncObjects();
		void recreateSwapchain();
		void cleanupSwapChain();


		void RecreateSwapChain();
		void RecreateCommandBuffers();
		void drawFrame();

		std::unique_ptr<Instance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;

		std::unique_ptr<SwapChain> m_SwapChain;
		std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline;
		
		std::unique_ptr<Buffer> m_Buffer;
		std::unique_ptr<DescriptorSetLayout> m_DescriptorSetLayout;
		std::unique_ptr<DescriptorPool> m_DescriptorPool;
		std::unique_ptr<DescriptorSet> m_DescriptorSets;

		std::unique_ptr<Renderer> m_Renderer;
		std::vector<std::unique_ptr<CommandBuffer>> m_CommandBuffers;
		std::map<std::thread::id, std::shared_ptr<CommandPool>> m_CommandPools;

		//Synchronisations primitives
		std::vector<VkSemaphore> m_ImageAvailableSemaphore;
		std::vector<VkSemaphore> m_RenderFinishedSemaphore;
		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;

		const int MAX_FRAMES_IN_FLIGHT = 2;


		size_t m_CurrentFrame = 0;
		size_t m_ImageIndex = 0;
		bool recreatingSwapchain = false;
		bool framebufferResized = false;

		std::unique_ptr<QuadModel> m_Quad;
	};
}