#pragma once
#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/Pipelines/Pipeline.h>
#include <Optimus/Graphics/Pipelines/Shader.h>

#include <Optimus/Graphics/RenderStage.h>

namespace OP
{
	class OPTIMUS_API GraphicsPipeline : public Pipeline
	{	
	public:

		enum class Mode
		{
			Polygon,
			MultipleRenderTargets //For Deferred Rendering, Post effects etc
		};

		enum class Depth
		{
			None = 0,
			Read = 1,
			Write = 2,
			ReadWrite = Read | Write
		};

		GraphicsPipeline(Stage stage, 
						std::vector<std::string> shaderStages, 
						std::vector<Shader::VertexInput> vertexInputs,
						Mode mode = Mode::Polygon, 
						Depth depth = Depth::ReadWrite, 
						VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
						VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL,
						VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
						VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE,
						bool pushDescriptors = false);
						

		~GraphicsPipeline();


		//Getters
		operator const VkPipeline &() const 
		{
			return m_GraphicsPipeline; 
		}
		const VkPipelineLayout& GetPipelineLayout()const { return m_PipelineLayout; }

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const override { return m_DescriptorSetLayout; }
		const VkPipeline& GetPipeline() const override { return m_GraphicsPipeline; }
		const VkPipelineBindPoint& GetPipelineBindPoint() const override { return m_PipelineBindPoint; }

	private:
		void createShaderProgram();
		void createDescriptorSetLayout();
		void createDescriptorPool();
		void createPipelineLayout();
		void createAttributes();

		void createPipeline();
		void createPolygonPipeline();
		void createMultipleRenderTargetsPipeline();

	private:

		Stage												m_Stage;
		std::vector<std::string>							m_ShaderPaths;
		std::vector<Shader::VertexInput>					m_VertexInputs;
		Mode												m_Mode;
		Depth												m_Depth;
		VkPrimitiveTopology									m_Topology;
		VkPolygonMode										m_PolygonMode;
		VkCullModeFlags										m_CullMode;
		VkFrontFace											m_FrontFace;

		std::unique_ptr<Shader>								m_Shader;
		std::vector<VkShaderModule>							m_ShaderModules;
		std::vector<VkPipelineShaderStageCreateInfo>		m_ShaderStages;

		std::vector<VkDynamicState>							m_DynamicStates	{ 
																			  VK_DYNAMIC_STATE_VIEWPORT, 
																			  VK_DYNAMIC_STATE_SCISSOR,
																			  VK_DYNAMIC_STATE_LINE_WIDTH
																			};

		bool												m_PushDescriptors;
		VkDescriptorSetLayout								m_DescriptorSetLayout			= VK_NULL_HANDLE;
		VkDescriptorPool									m_DescriptorPool				= VK_NULL_HANDLE;

		VkPipelineLayout									m_PipelineLayout				= {};
		VkPipeline											m_GraphicsPipeline				= {};
		VkPipelineBindPoint									m_PipelineBindPoint				= {};
		VkPipelineVertexInputStateCreateInfo				m_VertexInputStateCreateInfo	= {};
		VkPipelineInputAssemblyStateCreateInfo				m_InputAssemblyState			= {};
		VkPipelineRasterizationStateCreateInfo				m_RasterizationState			= {};
		std::array<VkPipelineColorBlendAttachmentState, 1>	m_BlendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo					m_ColourBlendState				= {};
		VkPipelineDepthStencilStateCreateInfo				m_DepthStencilState				= {};   //TODO
		VkPipelineViewportStateCreateInfo					m_ViewportState					= {};
		VkPipelineMultisampleStateCreateInfo				m_MultisampleState				= {};
		VkPipelineDynamicStateCreateInfo					m_DynamicState					= {};	//TODO
		VkPipelineTessellationStateCreateInfo				m_TessellationState				= {};	//TODO

		VkViewport											m_Viewport						= {};
		VkRect2D											m_Scissor						= {};


	};
}

