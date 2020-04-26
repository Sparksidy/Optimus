#include <pch.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Log.h>

namespace OP
{
	const std::vector<VkDynamicState> DYNAMIC_STATES = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };
	
	GraphicsPipeline::GraphicsPipeline(
		Stage stage,
		std::vector<std::string> shaderStages,
		std::vector<Shader::VertexInput> vertexInputs,
		Mode mode,
		Depth depth,
		VkPrimitiveTopology topology,
		VkPolygonMode polygonMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace,
		bool pushDescriptors):
		m_Stage(std::move(stage)),
		m_ShaderPaths(std::move(shaderStages)),
		m_VertexInputs(std::move(vertexInputs)),
		m_Mode(mode),
		m_Depth(depth),
		m_Topology(topology),
		m_PolygonMode(polygonMode),
		m_CullMode(cullMode),
		m_FrontFace(frontFace),
		m_Shader(std::make_unique<Shader>()),
		m_PipelineBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS),
		m_PushDescriptors(pushDescriptors)
	{	
		std::sort(m_VertexInputs.begin(), m_VertexInputs.end());
		createShaderProgram();
		//createDescriptorSetLayout();
		//createDescriptorPool();
		createPipelineLayout();
		createAttributes();

		switch (m_Mode)
		{
		case Mode::Polygon:
			createPolygonPipeline();
			break;
		case Mode::MultipleRenderTargets:
			createMultipleRenderTargetsPipeline();
			break;

		default:
			OP_CORE_FATAL("Unknown Pipeline mode");
			break;
		}
	}
	
	GraphicsPipeline::~GraphicsPipeline()
	{
		//auto& logicalDevice = GET_GRAPHICS_SYSTEM()->GetLogicalDevice();

		for (const auto& shaderModule : m_ShaderModules)
		{
			vkDestroyShaderModule(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), shaderModule, nullptr);
		}

		vkDestroyDescriptorPool(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_DescriptorPool, nullptr);
		vkDestroyPipeline(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_PipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_DescriptorSetLayout, nullptr);
	}
	

	void GraphicsPipeline::createShaderProgram()
	{
		for (const auto& shaderStage : m_ShaderPaths)
		{
			auto filecode = Shader::readFile(shaderStage);
			auto shaderModule = m_Shader->CreateShaderModule(filecode);
			
			const std::filesystem::path path = shaderStage;
			auto shaderStageFlag = m_Shader->GetShaderStage(path);

			VkPipelineShaderStageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info.stage = shaderStageFlag;
			info.module = shaderModule;
			info.pName = "main";
			
			m_ShaderStages.emplace_back(info);
			m_ShaderModules.emplace_back(shaderModule);
		}
	}

	void GraphicsPipeline::createDescriptorSetLayout()
	{
		auto& descriptorSetsLayouts = m_Shader->GetDescriptorSetLayouts();

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(descriptorSetsLayouts.size());
		layoutInfo.pBindings = descriptorSetsLayouts.data();

		OP_VULKAN_ASSERT(vkCreateDescriptorSetLayout,
						GET_GRAPHICS_SYSTEM()->GetLogicalDevice(),
						&layoutInfo,
						nullptr,
						&m_DescriptorSetLayout);
	}

	void GraphicsPipeline::createDescriptorPool()
	{
		int swapChainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();

		auto descriptorPools = m_Shader->GetDescriptorPools();

		VkDescriptorPoolCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		//info.flags = TODO
		info.maxSets = static_cast<uint32_t>(swapChainImages);
		info.poolSizeCount = static_cast<uint32_t>(descriptorPools.size());
		info.pPoolSizes = descriptorPools.data();

		OP_VULKAN_ASSERT(vkCreateDescriptorPool, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &info, nullptr, &m_DescriptorPool);
	}

	void GraphicsPipeline::createPipelineLayout()
	{
		//TODO for Push constants
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 0;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		//pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout; //TODO
		OP_VULKAN_ASSERT(vkCreatePipelineLayout, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout);
	}

	void GraphicsPipeline::createAttributes()
	{
		m_InputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_InputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_InputAssemblyState.primitiveRestartEnable = VK_FALSE;

		m_RasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_RasterizationState.depthClampEnable = VK_FALSE;
		m_RasterizationState.rasterizerDiscardEnable = VK_FALSE;
		m_RasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		m_RasterizationState.lineWidth = 1.0f;
		m_RasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
		m_RasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
		m_RasterizationState.depthBiasEnable = VK_FALSE;
		
		m_BlendAttachmentStates[0] = {};
		m_BlendAttachmentStates[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		m_BlendAttachmentStates[0].blendEnable = VK_FALSE;

		m_ColourBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_ColourBlendState.logicOpEnable = VK_FALSE;
		m_ColourBlendState.logicOp = VK_LOGIC_OP_COPY;
		m_ColourBlendState.attachmentCount = 1;
		m_ColourBlendState.pAttachments = m_BlendAttachmentStates.data();
		m_ColourBlendState.blendConstants[0] = 0.0f;
		m_ColourBlendState.blendConstants[1] = 0.0f;
		m_ColourBlendState.blendConstants[2] = 0.0f;
		m_ColourBlendState.blendConstants[3] = 0.0f;

		m_Viewport.x = 0.0f;
		m_Viewport.y = 0.0f;
		m_Viewport.width = (float)GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().width;
		m_Viewport.height = (float)GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().height;
		m_Viewport.minDepth = 0.0f;
		m_Viewport.maxDepth = 1.0f;

		m_Scissor.offset = { 0, 0 };
		m_Scissor.extent = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent();

		m_ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_ViewportState.viewportCount = 1;
		m_ViewportState.pViewports = &m_Viewport;
		m_ViewportState.scissorCount = 1;
		m_ViewportState.pScissors = &m_Scissor;

		m_MultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_MultisampleState.sampleShadingEnable = VK_FALSE;
		m_MultisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	}

	void GraphicsPipeline::createPipeline()
	{
		auto renderStage = GET_GRAPHICS_SYSTEM()->GetRenderStage(m_Stage.first);

		std::vector<VkVertexInputBindingDescription> bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;


		for (const auto& vertexInput : m_VertexInputs)
		{
			for (const auto& binding : vertexInput.GetBindingDescriptions())
			{
				bindingDescriptions.emplace_back(binding);
			}

			for (const auto& attribute : vertexInput.GetAttributeDescriptions())
			{
				attributeDescriptions.emplace_back(attribute);
			}

		}

		m_VertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		m_VertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		m_VertexInputStateCreateInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		m_VertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		m_VertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(m_ShaderStages.size());
		pipelineInfo.pStages = m_ShaderStages.data();
		pipelineInfo.pVertexInputState = &m_VertexInputStateCreateInfo;
		pipelineInfo.pInputAssemblyState = &m_InputAssemblyState;
		pipelineInfo.pViewportState = &m_ViewportState;
		pipelineInfo.pRasterizationState = &m_RasterizationState;
		pipelineInfo.pMultisampleState = &m_MultisampleState;
		pipelineInfo.pColorBlendState = &m_ColourBlendState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = *renderStage->GetRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		OP_VULKAN_ASSERT(vkCreateGraphicsPipelines, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);

		OP_CORE_INFO("Graphics Pipeline created");
	}

	void GraphicsPipeline::createPolygonPipeline()
	{
		createPipeline();
	}

	void GraphicsPipeline::createMultipleRenderTargetsPipeline()
	{
		//TODO
	}

	
}