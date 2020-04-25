#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Pipelines/Pipeline.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Utilities/TypeInfo.h>

namespace OP
{
	//Base Class: Render Pipeline to render a particular type of pipeline
	class OPTIMUS_API SubRender
	{
	public:
		explicit SubRender(Pipeline::Stage stage):
			m_Stage(std::move(stage)){}

		virtual ~SubRender() = default;

		virtual void Render(const CommandBuffer& commandBuffer) = 0;

		const Pipeline::Stage& GetPipelineStage()const { return m_Stage; }
		bool IsEnabled()const { return m_Enabled; }
		void SetEnabled(bool value) { m_Enabled = value; }

	private:
		bool m_Enabled = true;
		Pipeline::Stage m_Stage;
	};

	template class OPTIMUS_API TypeInfo<SubRender>;

	template <typename T>
	TypeId GetSubRenderTypeID() noexcept {
		static_assert(std::is_base_of<SubRender, T>::value, "T must be a Subrender.");

		return TypeInfo<SubRender>::GetTypeID<T>();
	}
}
