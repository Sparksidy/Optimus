#include <pch.h>
#include <Optimus/Graphics/Models/QuadModel.h>
#include <Optimus/Graphics/Buffers/Buffer.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Log.h>

namespace OP
{
	QuadModel::QuadModel()
	{
		SetVertices();
		SetIndices();
	}
	QuadModel::~QuadModel()
	{
	}
	bool QuadModel::CmdRender(const CommandBuffer& commandBuffer, uint32_t instances) const
	{
		if (m_VertexBuffer && m_IndexBuffer)
		{
			VkBuffer vertexBuffers[1] = { m_VertexBuffer->GetBuffer() };
			VkDeviceSize offsets[1] = { 0 } ;
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandBuffer, m_IndexCount, instances, 0, 0, 0);
		}
		else if (m_VertexBuffer && !m_IndexBuffer)
		{
			VkBuffer vertexBuffers[1] = { m_VertexBuffer->GetBuffer() };
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			vkCmdDraw(commandBuffer, m_VerticesCount, instances, 0, 0);
		}
		else
		{
			OP_CORE_FATAL("Model with no buffers cannot be rendered");
			return false;
		}

		return true;
	}
	void QuadModel::SetVertices()
	{
		m_VertexBuffer = nullptr;
		m_VerticesCount = static_cast<uint32_t>(m_Vertices.size());

		//Create a staging buffer
		Buffer vertexStaging(sizeof(Vertex2d) * m_Vertices.size(),
							 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
							 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
							 m_Vertices.data());

		//Create the vertex Buffer
		m_VertexBuffer = std::make_unique<Buffer>(vertexStaging.GetSize(),
												VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
												VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		//Submit the command to copy staging to vertex buffer
		CommandBuffer commandBuffer;
		VkBufferCopy copyRegion = {};
		copyRegion.size = vertexStaging.GetSize();
		vkCmdCopyBuffer(commandBuffer, vertexStaging.GetBuffer(), m_VertexBuffer->GetBuffer(), 1, &copyRegion);

		commandBuffer.SubmitIdle();
	}
	void QuadModel::SetIndices()
	{
		m_IndexBuffer = nullptr;
		m_IndexCount = m_Indices.size();

		//Create the staging buffer
		Buffer indexStaging(sizeof(uint32_t) * m_Indices.size(),
							VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
							VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
							m_Indices.data());

		//Create the index Buffer
		m_IndexBuffer = std::make_unique<Buffer>(indexStaging.GetSize(),
												VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
												VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


		//Submit the command to copy staging to index buffer
		CommandBuffer commandBuffer;
		VkBufferCopy copyRegion = {};
		copyRegion.size = indexStaging.GetSize();
		vkCmdCopyBuffer(commandBuffer, indexStaging.GetBuffer(), m_IndexBuffer->GetBuffer(), 1, &copyRegion);

		commandBuffer.SubmitIdle();
	}
}