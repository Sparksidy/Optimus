#include <pch.h>

#include <chrono>
#include <Optimus/Graphics/Buffers/Buffer.h>

#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace OP
{
	Buffer::Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices):
		m_Vertices(std::move(vertices)),
		m_Indices(std::move(indices))
	{
		//Texture Stuff
		createTextureImage();
		createTextureImageView();
		createTextureSampler();

		createVertexBuffer();
		createIndexBuffer();
		createUniformBuffer();
	}

	Buffer::~Buffer()
	{
		//Cleanup the vertex Buffer

		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_indexBuffer, nullptr);
		OP_CORE_INFO("Index Buffer Destroyed");

		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_indexBufferMemory, nullptr);
		OP_CORE_INFO("Index Buffer Memory Freed");

		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_vertexBuffer, nullptr);
		OP_CORE_INFO("Vertex Buffer Destroyed");

		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_vertexBufferMemory, nullptr);
		OP_CORE_INFO("Vertex Buffer Memory Freed");

		vkDestroySampler(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_TextureSampler, nullptr);
		OP_CORE_INFO("Texture Sampler Memory Freed");

		vkDestroyImageView(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_TextureImageView, nullptr);
		OP_CORE_INFO("Texture Image View Freed");

		vkDestroyImage(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_TextureImage, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_TextureImageMemory, nullptr);
		OP_CORE_INFO("Texture Image Memory Freed");
	}

	void Buffer::FreeAndDestroyUniformBuffers()
	{
		int size = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();
		for (size_t i = 0; i < size; i++)
		{
			vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_uniformBuffer[i], nullptr);
			vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(),m_uniformBufferMemory[i], nullptr);
		}
	}

	void Buffer::RecreateUniformBuffers()
	{
		//createUniformBuffer();
	}

	void Buffer::UpdateUniformBuffers(uint32_t currentImage)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo = {};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().width /(float) GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_uniformBufferMemory[currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_uniformBufferMemory[currentImage]);
	}

	uint32_t Buffer::findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties)
	{
		auto memoryProperties = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetPhysicalDevice().GetMemoryProperties();

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
			uint32_t memoryTypeBits = 1 << i;

			if (typeFilter & memoryTypeBits && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("Failed to find a valid memory type for buffer");
	}

	void Buffer::createBuffer(const VkDeviceSize& size, 
									const VkBufferUsageFlags& usage,
									const VkMemoryPropertyFlags& memoryProperties, 
									VkBuffer& Buffer, 
									VkDeviceMemory& BufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		OP_VULKAN_ASSERT(vkCreateBuffer, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &bufferInfo, nullptr, &Buffer);
		OP_CORE_INFO("Buffer successfully created");

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), Buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);

		OP_VULKAN_ASSERT(vkAllocateMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, nullptr, &BufferMemory);
		OP_CORE_INFO("Buffer Memory successfully allocated");

		OP_VULKAN_ASSERT(vkBindBufferMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), Buffer, BufferMemory, 0);
		OP_CORE_INFO("Buffer memory binded to the buffer");
	}

	void Buffer::createTextureImage()
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load("C:/Users/sidys/OneDrive/Desktop/Optimus/Optimus/src/Optimus/Graphics/Textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize =  texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!");
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		//Copy pixel values into the staging buffer
		void* data;
		vkMapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory);

		stbi_image_free(pixels);

		createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

		//Transition the image to a layout that's optimal
		transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		//Copy the staging buffer to the texture image
		copyBufferToImage(stagingBuffer, m_TextureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

		//Transition to be able to sample from the shader
		transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void Buffer::createTextureImageView()
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_TextureImage;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		OP_VULKAN_ASSERT(vkCreateImageView, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &viewInfo, nullptr, &m_TextureImageView);
		OP_CORE_INFO("Texture Image View Created");
	}

	void Buffer::createTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;

		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;

		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		OP_VULKAN_ASSERT(vkCreateSampler, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &samplerInfo, nullptr, &m_TextureSampler);
		OP_CORE_INFO("Texture Samplers are created");
	}

	void Buffer::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
	{
		//Create the texture image
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		OP_VULKAN_ASSERT(vkCreateImage, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &imageInfo, nullptr, &image);
		OP_CORE_INFO("Texture Image has been created");

		//Allocate memory for the image
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		OP_VULKAN_ASSERT(vkAllocateMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, nullptr, &imageMemory);
		OP_CORE_INFO("Image Memory allocated");

		vkBindImageMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), image, imageMemory, 0);
	}

	void Buffer::createVertexBuffer()
	{
		OP_CORE_INFO("Creating Vertex Buffer");
		VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_Vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		OP_VULKAN_ASSERT(vkMapMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		std::memcpy(data, m_Vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

		copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

		//Clean up Staging buffer
		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void Buffer::createIndexBuffer()
	{
		OP_CORE_INFO("Creating Index Buffer");
		VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_Indices.data(), (size_t)bufferSize);
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

		copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void Buffer::createUniformBuffer()
	{
		//Create uniform buffers per swapchain image
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		int swapChainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();

		m_uniformBuffer.resize(swapChainImages);
		m_uniformBufferMemory.resize(swapChainImages);

		for (size_t i = 0; i < swapChainImages; i++)
		{
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffer[i], m_uniformBufferMemory[i]);
		}

		OP_CORE_INFO("Uniform Buffers Created");
	}

	void Buffer::copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size)
	{	
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	VkCommandBuffer Buffer::beginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GET_GRAPHICS_SYSTEM()->GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void Buffer::endSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue());

		vkFreeCommandBuffers(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), GET_GRAPHICS_SYSTEM()->GetCommandPool(), 1, &commandBuffer);
	}

	void Buffer::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		endSingleTimeCommands(commandBuffer);
	}

	void Buffer::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

			VkBufferImageCopy region = {};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;

			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;

			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = {
				width,
				height,
				1
			};

			vkCmdCopyBufferToImage(
				commandBuffer,
				buffer,
				image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&region
			);

		endSingleTimeCommands(commandBuffer);
	}

}