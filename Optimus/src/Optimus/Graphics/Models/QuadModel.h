#pragma once
#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Models/Vertex2d.h>

namespace OP
{
	class Buffer;
	class Vertex2d;
	class CommandBuffer;

	class OPTIMUS_API QuadModel
	{
	public:
		QuadModel();

		~QuadModel();

		bool CmdRender(const CommandBuffer& commandBuffer, uint32_t instances = 1)const;

	private:
		void SetVertices();
		void SetIndices();

	private:
		const std::vector<Vertex2d> m_Vertices = {
			//pos			//Color				
		   {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint16_t> m_Indices = 
		{
			0, 1, 2, 2, 3, 0
		};

		std::unique_ptr<Buffer> m_VertexBuffer;
		std::unique_ptr<Buffer> m_IndexBuffer;

		uint32_t m_VerticesCount;
		uint32_t m_IndexCount;
	};
}