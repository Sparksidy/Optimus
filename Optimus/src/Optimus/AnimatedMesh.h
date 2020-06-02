#pragma once
#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>


#include <Optimus/Graphics/Core/SubRender.h>
#include <Optimus/Graphics/Models/QuadModel.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Graphics/Images/Image2D.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Application.h>
#include <Optimus/Log.h>

#include <Optimus/Mesh.h>

namespace OP
{
	class OPTIMUS_API AnimatedMesh : public Mesh
	{
	public:

		AnimatedMesh(const char* filename, float framesPerElement, float frames, float duration) :
			Mesh(filename),
			m_FramesPerElement(framesPerElement),
			m_Frames(frames),
			m_SecondsPerFrame(duration / frames),
			m_Counter(0.0f),
			m_CurrentFrame(0),
			m_CurrTime(0.0f)
		{
			
			m_FrameSize.x = 1.0f / (float)((m_Frames + (m_FramesPerElement - 1)) / m_FramesPerElement);
			m_FrameSize.y = 1.0f / (float)m_FramesPerElement;
			
			m_UniformHandler.SetMode(1);
		}

		~AnimatedMesh()
		{
			OP_CORE_INFO("Destroying Mesh");
		}

		virtual void Render(const CommandBuffer& commandBuffer, const Pipeline& pipeline)override
		{
			if (!m_DescriptorHandler.Update(m_UniformHandler, *m_Image, pipeline))
				return;

			ConstructTextureMatrix();

			UpdateSprite();

			//Updates the uniform
			m_UniformHandler.Update();

			//Bind the descriptors
			m_DescriptorHandler.BindDescriptor(commandBuffer);

			//Render the quad TODO: (Loop through all the meshes in the scene instead of one quad)
			m_Quad->CmdRender(commandBuffer);
		}

		void ConstructTextureMatrix()
		{
			glm::vec2 const& scale = m_FrameSize;
			float x, y;

			x = (float)(m_CurrentFrame / m_FramesPerElement) * scale.x;
			y = 1.0f - ((float)(((m_CurrentFrame % m_FramesPerElement)) + 1) * scale.y);

			glm::mat3 textureMatrix;
			textureMatrix[0][0] = scale.x;		textureMatrix[0][1] = 0;			textureMatrix[0][2] = x;
			textureMatrix[1][0] = 0;			textureMatrix[1][1] = scale.y;		textureMatrix[1][2] = y;
			textureMatrix[2][0] = 0;			textureMatrix[2][1] = 0;			textureMatrix[2][2] = 1;

			textureMatrix = glm::transpose(textureMatrix);

			m_UniformHandler.SetTextureMatrix(textureMatrix);
		}


		void UpdateSprite()
		{
			float time = (float)glfwGetTime();
			float deltaTime = m_CurrTime > 0.0f ? time - m_CurrTime : (1.0f / 60.0f);
			m_CurrTime = time;

			m_Counter += deltaTime;
			if (m_Counter >= m_SecondsPerFrame)
			{
				NextFrame();
				m_Counter = 0.0f;
			}
		}

		void NextFrame()
		{
			m_CurrentFrame = (m_CurrentFrame + 1) % m_Frames;
		}

	private:
		unsigned int	m_FramesPerElement;
		unsigned int	m_Frames;
		glm::vec2		m_FrameSize;
		float			m_SecondsPerFrame;

		float			m_Counter;
		unsigned int	m_CurrentFrame;
		float			m_CurrTime = 0.0f;

	};

}