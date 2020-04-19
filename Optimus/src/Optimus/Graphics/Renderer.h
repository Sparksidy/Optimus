#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/RenderStage.h>

namespace OP
{
	class OPTIMUS_API Renderer
	{
		friend class Graphics;
	public:
		Renderer() = default;

		virtual ~Renderer() = default;

		virtual void Start() = 0;

		virtual void Update() = 0;

		RenderStage* GetRenderStage(uint32_t index)const
		{
			if (m_RenderStages.empty() || m_RenderStages.size() < index)
				return nullptr;

			return m_RenderStages.at(index);
		}
	protected:
		void AddRenderStage(std::vector<Attachment> renderpassAttachment, std::vector<SubpassType> renderpassSubpasses)
		{
			m_RenderStages.emplace_back(new RenderStage(renderpassAttachment, renderpassSubpasses));
		}

	private:
		bool m_Started = false;
		std::vector<RenderStage*> m_RenderStages;
	};
}