#pragma once
#include <Optimus/Core.h>
#include <Optimus/Layer.h>

#include <vulkan/vulkan.hpp>
#include <Optimus/Events/MouseEvents.h>
#include <Optimus/Events/KeyboardEvents.h>

namespace OP
{
	class OPTIMUS_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		void OnAttach()override;
		void OnDetach()override;
		void OnUpdate()override;
		void OnEvent(Event&)override;

		bool OnMouseButtonPressedEvent(MouseButtonPressed&);
		bool OnMouseButtonReleasedEvent(MouseButtonReleased&);
		bool OnMouseScrollEvent(MouseScroll&);
		bool OnMouseMoveEvent(MouseMove&);
		bool OnKeyPressedEvent(KeyPressedEvent&);
		bool OnKeyReleasedEvent(KeyReleasedEvent&);
		bool OnKeyTypedEvent(KeyTypedEvent&);

		static void ShowSimpleOverlay(bool* p_open);

		std::vector<VkCommandBuffer>& GetImGUICommandBuffer()  { return m_ImguiCommandBuffer; } //Used to add to the main command buffers

	private:
		//Helper functions
		void _createIMGUIRenderPass();
		void _createIMGUIDescriptorPool();
		void _initIMGUIContext();
		void _initIMGUIVulkan();
		void _uploadFonts();
		void _createIMGUICommandPoolsAndBuffers();
		void _createFramebuffers();

		void FrameRender();

	private:
		float m_time = 0.0f;

		VkRenderPass						m_ImguiRenderPass;
		VkCommandPool						m_ImguiCommandPool;
		VkDescriptorPool					m_ImguiDescriptorPool;

		std::vector<VkCommandBuffer>		m_ImguiCommandBuffer;
		std::vector<VkFramebuffer>			m_ImguiFrameBuffers;
	};
}