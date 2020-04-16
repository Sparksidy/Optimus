#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class RenderStage;
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

			return m_RenderStages.at(index).get();
		}

		void AddRenderStage(std::unique_ptr<RenderStage>&& renderstage)
		{
			m_RenderStages.emplace_back(std::move(renderstage));
		}

	private:
		bool m_Started = false;
		std::vector<std::unique_ptr<RenderStage>> m_RenderStages;
	};
}