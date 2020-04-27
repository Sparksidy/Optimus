#include <pch.h>
#include <Optimus/Graphics/Core/SubRenderHolder.h>

namespace OP
{
	void SubrenderHolder::RemoveSubRenderStage(const TypeId& id)
	{
		for (auto it = m_Stages.begin(); it != m_Stages.end();)
		{
			if (it->second == id)
				m_Stages.erase(it);
			else
				++it;
		}
	}
	void SubrenderHolder::RenderStage(const Pipeline::Stage& stage, const CommandBuffer& commandBuffer)
	{
		for (const auto& [stageIndex, typeId] : m_Stages)
		{
			if (stageIndex.first != stage)
				continue;

			if (auto& subrender = m_SubRenders[typeId])
			{
				if (subrender->IsEnabled())
				{
					subrender->Render(commandBuffer);
				}
			}
		}
	}
}
