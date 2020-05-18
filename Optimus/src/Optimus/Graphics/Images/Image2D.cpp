#include <pch.h>
#include <Optimus/Graphics/Images/Image2D.h>
#include <Optimus/Graphics/Images/Image.h>
#include <Optimus/Application.h>
#include <Optimus/Log.h>
#include <Optimus/Graphics/Buffers/Buffer.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace OP
{
	Image2D::Image2D(const char* filename, VkFilter filter, VkSamplerAddressMode addressMode,
		bool anisotropic, bool mipmap, bool load) :
		m_FileName(std::move(filename)),
		m_Filter(filter),
		m_AddressMode(addressMode),
		m_Anisotropic(anisotropic),
		m_Mipmap(mipmap),
		m_Samples(VK_SAMPLE_COUNT_1_BIT),
		m_Layout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
		m_Usage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		m_Format(VK_FORMAT_R8G8B8A8_UNORM)
	{
		if (load)
		{
			Image2D::Load();
		}
	}

	Image2D::~Image2D()
	{
		vkDestroySampler(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Sampler, nullptr);
		vkDestroyImageView(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_View, nullptr);

		vkDestroyImage(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Image, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Memory, nullptr);

		OP_CORE_INFO("Destroyed Image Resources");
	}

	WriteDescriptorSet Image2D::GetWriteDescriptorSet(uint32_t binding) const
	{
		//Image
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = m_View; 
		imageInfo.sampler = m_Sampler;
		VkWriteDescriptorSet descriptorWrite2{};
		descriptorWrite2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite2.dstBinding = binding;
		descriptorWrite2.dstArrayElement = 0;
		descriptorWrite2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite2.descriptorCount = 1;
		descriptorWrite2.pImageInfo = &imageInfo;

		return { descriptorWrite2, imageInfo };
	}

	void Image2D::Load()
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(m_FileName, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels)
		{
			throw std::runtime_error("failed to load texture image!");
		}

		OP_CORE_INFO("Pixels are loaded from file");

		Buffer stagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		stagingBuffer.MapMemory(&data);
		std::memcpy(data, pixels, static_cast<size_t>(imageSize));
		stagingBuffer.UnmapMemory();

		stbi_image_free(pixels);

		VkExtent3D extents = { texWidth, texHeight, 1 };

		Image::CreateImage(m_Image, m_Memory, extents, m_Format, m_Samples, VK_IMAGE_TILING_OPTIMAL, m_Usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			1, 1, VK_IMAGE_TYPE_2D);

		Image::TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1, 0);
		Image::CopyBufferToImage(stagingBuffer.GetBuffer(), m_Image, extents, 1, 0);
		Image::TransitionImageLayout(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, 1, 0);

		OP_CORE_INFO("Image Created and Transitioned");

		Image::CreateImageSampler(m_Sampler, m_Filter, m_AddressMode, m_Anisotropic, m_mipLevels);
	
		OP_CORE_INFO("Image Sampler has been created");

		Image::CreateImageView(m_Image, m_View, VK_IMAGE_VIEW_TYPE_2D, m_Format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels, 0, 1, 0);

		OP_CORE_INFO("Image View has been created");
	}
}