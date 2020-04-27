#include <pch.h>
#include <Optimus/Graphics/Core/RenderStage.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Core/Graphics.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>

namespace OP
{
	RenderStage::RenderStage(std::vector<Attachment> images, std::vector<SubpassType> subpasses, const Viewport& viewport):
		m_Attachments(std::move(images)),
		m_Subpasses(std::move(subpasses)),
		m_Viewport(viewport)
	{

		for (const auto& image : m_Attachments)
		{
			switch (image.GetType())
			{
			case Attachment::Type::Image:
				//TODO
				break;
			case Attachment::Type::Depth:
				//TODO
				break;
			case Attachment::Type::Swapchain:
				m_SwapchainAttachment = image;
				break;
			}
		}
	}

	void RenderStage::Update()
	{
		//Check if the renderstage is out of date here
		auto lastRenderArea = m_RenderArea;

		m_RenderArea.SetOffset(m_Viewport.GetOffset());

		if (m_Viewport.GetSize())
			m_RenderArea.SetExtent(m_Viewport.GetScale() * *m_Viewport.GetSize());
		else
			m_RenderArea.SetExtent(m_Viewport.GetScale() * glm::vec2(GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().width, GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().height));

		m_RenderArea.SetAspectRatio(static_cast<float>(m_RenderArea.GetExtent().x) / static_cast<float>(m_RenderArea.GetExtent().y));
		m_RenderArea.SetExtent(m_RenderArea.GetExtent() + m_RenderArea.GetOffset());

		m_outOfDate = m_RenderArea != lastRenderArea;
	}

	void RenderStage::Rebuild(const SwapChain& swapchain)
	{
		//If render stage out of date then rebuild stages here
		Update();

		if (!m_RenderPass)
		{
			m_RenderPass = std::make_unique<RenderPass>(*this, VK_FORMAT_UNDEFINED, GET_GRAPHICS_SYSTEM()->GetSurface().GetFormat().format, GET_GRAPHICS_SYSTEM()->GetPhysicalDevice().GetMsaaSamples());
		}

		m_FrameBuffers = std::make_unique<Framebuffers>(*this, GET_GRAPHICS_SYSTEM()->GetSwapchain());

		m_outOfDate = false;

		OP_CORE_INFO("Render Stage Created");
	}

	std::optional<Attachment> RenderStage::GetAttachment(const std::string& name) const
	{
		auto it = std::find_if(m_Attachments.begin(), m_Attachments.end(), [name](const Attachment& a){
			return a.GetName() == name;
			});

		if (it == m_Attachments.end())
			return std::nullopt;

		return *it;
	}

	std::optional<Attachment> RenderStage::GetAttachment(uint32_t binding) const
	{
		auto it = std::find_if(m_Attachments.begin(), m_Attachments.end(), [binding](const Attachment& ref) {
			return ref.GetBinding() == binding;
			});

		if (it == m_Attachments.end())
			return std::nullopt;

		return *it;
	}
	const VkFramebuffer& RenderStage::GetActiveFrameBuffer(uint32_t activeswapChainImage) const
	{
		if (activeswapChainImage > m_FrameBuffers->GetFramebuffers().size())
		{
			return m_FrameBuffers->GetFramebuffers().at(0);
		}

		return m_FrameBuffers->GetFramebuffers().at(activeswapChainImage);
	}
}