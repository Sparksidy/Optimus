#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Utilities/NonCopyable.h>
#include <Optimus/Graphics/Pipelines/Pipeline.h>
#include <Optimus/Utilities/TypeInfo.h>
#include <Optimus/Graphics/Core/SubRender.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>

namespace OP
{
	/*
		Class that contains and manages the subrenders submitted to the Renderer
	*/
	class OPTIMUS_API SubrenderHolder : public NonCopyable
	{
		friend class Graphics;

	public:

		template <typename T>
		T* Get() const
		{
			const auto id = GetSubRenderTypeID<T>();

			auto it = m_SubRenders.find(id);

			if (it == m_SubRenders.end() || !it->second)
			{
				return nullptr;
			}

			return static_cast<T*>(it->second.get());
		}
		
		template <typename T>
		T* Add(const Pipeline::Stage& stage, std::unique_ptr<SubRender>&& subrender)
		{
			const auto id = GetSubRenderTypeID<T>();

			m_Stages.insert({ StageIndex(stage, m_SubRenders.size()), id });

			m_SubRenders[id] = std::move(subrender);
			return static_cast<T*>(m_SubRenders[id].get());
		}

		template <typename T>
		void Remove()
		{
			const auto typeId = GetSubRenderTypeID<T>();

			RemoveSubRenderStage();

			m_SubRenders.erase(typeId);
		}

		void Clear()
		{
			m_Stages.clear();
		}


	private:

		void RemoveSubRenderStage(const TypeId& id);

		void RenderStage(const Pipeline::Stage& stage, const CommandBuffer& commandBuffer);


	private:

		using StageIndex = std::pair<Pipeline::Stage, std::size_t>;

		std::unordered_map<TypeId, std::unique_ptr<SubRender>> m_SubRenders;

		std::multimap<StageIndex, TypeId> m_Stages;
	};
}
