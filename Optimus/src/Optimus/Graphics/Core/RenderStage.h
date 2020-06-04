#pragma once
#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/RenderPass/Framebuffers.h> 
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>

#include <Optimus/Log.h>

#include <optional>

namespace OP
{
	/*Represents an attachment in Render Pass*/
	class OPTIMUS_API Attachment
	{
	public:
		enum class Type {Image, Depth, Swapchain};

		Attachment(uint32_t binding, std::string name, Type type, bool multisampled = false, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM) :
			m_Binding(binding),
			m_Name(std::move(name)),
			m_Type(type),
			m_MultiSampled(multisampled),
			m_Format(format)
			{}

		//Getters
		uint32_t GetBinding() const { return m_Binding; }
		const std::string& GetName() const { return m_Name; }
		Type GetType() const { return m_Type; }
		bool IsMultisampled() const { return m_MultiSampled; }
		VkFormat GetFormat() const { return m_Format; }

	private:
		uint32_t		m_Binding;
		std::string		m_Name;
		Type			m_Type;
		bool			m_MultiSampled;
		VkFormat		m_Format;
	};

	class OPTIMUS_API SubpassType
	{
	public:
		SubpassType(uint32_t binding, std::vector<uint32_t> attachmentBindings) :
			m_Binding(binding),
			m_AttachmentBindings(std::move(attachmentBindings)) {
		}

		//Getters
		uint32_t GetBinding() const { return m_Binding; }
		const std::vector<uint32_t>& GetAttachmentBindings() const { return m_AttachmentBindings; }

	private:
		uint32_t m_Binding;
		std::vector<uint32_t> m_AttachmentBindings;
	}; 

	class OPTIMUS_API RenderArea
	{
	public:
		explicit RenderArea(const glm::vec2& extent = {}, const glm::vec2& offset = {}) :
			m_Extent(extent),m_Offset(offset) {
		}

		bool operator==(const RenderArea& other) const
		{
			return m_Extent == other.m_Extent && m_Offset == other.m_Offset;
		}

		bool operator!=(const RenderArea& other) const
		{
			return !operator==(other);
		}

		const glm::vec2& GetExtent() const { return m_Extent; }
		void SetExtent(const glm::vec2& extent) { m_Extent = extent; }

		const glm::vec2& GetOffset() const { return m_Offset; }
		void SetOffset(const glm::vec2& offset) { m_Offset = offset; }

		/**
		 * Gets the aspect ratio between the render stages width and height.
		 * @return The aspect ratio.
		 */
		float GetAspectRatio() const { return m_aspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }

	private:
		glm::vec2 m_Extent;
		glm::vec2 m_Offset;
		float m_aspectRatio = 1.0f;
	};

	class OPTIMUS_API Viewport
	{
	public:
		Viewport() = default;

		explicit Viewport(const glm::vec2& size) :
			m_Size(size) {
		}

		const glm::vec2& GetScale() const { return m_scale; }
		void SetScale(const glm::vec2& scale) { m_scale = scale; }

		const std::optional<glm::vec2>& GetSize() const { return m_Size; }
		void SetSize(const std::optional<glm::vec2>& size) { m_Size = size; }

		const glm::vec2& GetOffset() const { return m_Offset; }
		void SetOffset(const glm::vec2& offset) { m_Offset = offset; }

	private:
		glm::vec2 m_scale = { 1.0f, 1.0f };
		std::optional<glm::vec2> m_Size;
		glm::vec2 m_Offset;
	};

	class OPTIMUS_API RenderStage
	{
		friend class Graphics;
	public:
		explicit RenderStage(std::vector<Attachment> images = {}, std::vector<SubpassType> subpasses = {}, const Viewport& viewport = Viewport());
		~RenderStage() 
		{
			OP_CORE_INFO("Destroying RenderStage");
		}

		void Update();
		void Rebuild(const SwapChain& swapchain);

		std::optional<Attachment> GetAttachment(const std::string& name)const;
		std::optional<Attachment> GetAttachment(uint32_t binding)const;

		const std::vector<Attachment>& GetAttachments() const { return m_Attachments; }
		const std::vector<SubpassType>& GetSubpasses() const { return m_Subpasses; }

		const RenderPass* GetRenderPass() const { return m_RenderPass.get(); }
		const Framebuffers* GetFramebuffers() const { return m_FrameBuffers.get(); }
		const VkFramebuffer& GetActiveFrameBuffer(uint32_t swapChainImage)const;

		Viewport& GetViewport() { return m_Viewport; }
		void SetViewport(const Viewport& viewport) { m_Viewport = viewport; }

		bool IsOutOfDate() const { return m_outOfDate; }
		const RenderArea& GetRenderArea() const { return m_RenderArea; }
		bool HasSwapChain()const { return m_SwapchainAttachment.has_value(); }

	private:
		std::vector<Attachment>			m_Attachments;
		std::vector<SubpassType>		m_Subpasses;

		Viewport						m_Viewport;

		std::unique_ptr<OP::RenderPass>		m_RenderPass;	
		std::unique_ptr<OP::Framebuffers>	m_FrameBuffers;

		std::optional<Attachment>		m_SwapchainAttachment;

		RenderArea						m_RenderArea;
		bool							m_outOfDate = false;
	};
}