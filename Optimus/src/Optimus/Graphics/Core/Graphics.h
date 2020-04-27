#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>
#include <Optimus/ISystem.h>

#include <Optimus/Graphics/Devices/Instance.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/Models/QuadModel.h>

#include <Optimus/Graphics/Core/Renderer.h>


namespace OP
{
	class OPTIMUS_API Graphics : public ISystem
	{
	public:
		Graphics();

		~Graphics();

		bool Initialize() override { return true; }
		void Update() override;
		void Unload() override; 

		inline std::string GetName()const { return "Graphics"; }

		//Getters
		const Instance& GetInstance() const { return *m_Instance.get(); }
		const Surface& GetSurface() const { return *m_Surface.get(); }
		const PhysicalDevice& GetPhysicalDevice() const { return *m_PhysicalDevice.get(); }
		const LogicalDevice& GetLogicalDevice() const { return *m_LogicalDevice.get(); }
		const SwapChain& GetSwapchain()const { return *m_SwapChain.get(); }
		Renderer* GetRenderer()const { return m_Renderer.get(); }
		RenderStage* GetRenderStage(uint32_t index);
		const std::shared_ptr<CommandPool>& GetCommandPool(const std::thread::id& id = std::this_thread::get_id());

		//Setters
		void SetRenderer(std::unique_ptr<Renderer>&& renderer) { m_Renderer = std::move(renderer); }

	private:
		void CreatePipelineCache();
		void ResetRenderStages();
		void RecreateSwapChain();
		void RecreateCommandBuffers();
		bool StartRenderPass(RenderStage& renderStage);
		bool EndRenderPass(RenderStage& renderStage);
		void DrawFrame();

		std::unique_ptr<Instance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;

		std::unique_ptr<SwapChain> m_SwapChain;

		std::unique_ptr<Renderer> m_Renderer;
		std::vector<std::unique_ptr<CommandBuffer>> m_CommandBuffers;
		std::map<std::thread::id, std::shared_ptr<CommandPool>> m_CommandPools;
		VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;

		//Synchronisations primitives
		std::vector<VkSemaphore> m_ImageAvailableSemaphore;
		std::vector<VkSemaphore> m_RenderFinishedSemaphore;
		std::vector<VkFence> m_InFlightFences;


		const int MAX_FRAMES_IN_FLIGHT = 2;
		size_t m_CurrentFrame = 0;
		size_t m_ImageIndex = 0;

	};
}