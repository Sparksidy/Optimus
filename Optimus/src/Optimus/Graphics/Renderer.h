#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/RenderStage.h>
#include <Optimus/Graphics/SubRenderHolder.h>

namespace OP
{
	/*
		Manages Subrenders objects and create a list of render passes
	*/
	class OPTIMUS_API Renderer
	{
		friend class Graphics;
	public:
		Renderer() = default;

		virtual ~Renderer() = default;

		virtual void Start() = 0;

		virtual void Update() = 0;

		template <typename T>
		T* Get()const
		{
			return m_SubRenderHolder.Get<T>();
		}

		RenderStage* GetRenderStage(uint32_t index)const
		{
			if (m_RenderStages.empty() || m_RenderStages.size() < index)
				return nullptr;

			return m_RenderStages.at(index);
		}
	protected:

		//Variadic Templates
		template <typename T>
		T* AddSubRender(const Pipeline::Stage& stage)
		{
			return m_SubRenderHolder.Add<T>(stage, std::make_unique<T>(stage));
		}

		template <typename T>
		void RemoveSubRender()
		{
			m_SubRenderHolder.Remove();
		}

		template <typename T>
		void Clear()
		{
			m_SubRenderHolder.Clear();
		}


		void AddRenderStage(std::vector<Attachment> renderpassAttachment, std::vector<SubpassType> renderpassSubpasses)
		{
			m_RenderStages.emplace_back(new RenderStage(renderpassAttachment, renderpassSubpasses));
		}

	private:
		bool m_Started = false;
		std::vector<RenderStage*> m_RenderStages;

		SubrenderHolder m_SubRenderHolder;
	};
}