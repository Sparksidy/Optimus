#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Descriptors/Descriptor.h>

namespace OP
{
	class OPTIMUS_API Image2D : public Descriptor
	{
	public:
		explicit Image2D(const char* filename,VkFilter filter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT, bool anisotropic = true, bool mipmap = true, bool load = true);

		const VkSampler& GetSampler() const { return m_Sampler; }
		const VkImageView& GetView() const { return m_View; }

		~Image2D();

		WriteDescriptorSet GetWriteDescriptorSet(uint32_t binding)const override;

	private:
		void Load();

	private:
		const char* m_FileName;

		VkFilter m_Filter;
		VkSamplerAddressMode m_AddressMode;
		bool m_Anisotropic;
		bool m_Mipmap;
		VkSampleCountFlagBits m_Samples;
		VkImageLayout m_Layout;
		VkImageUsageFlags m_Usage;
		VkFormat m_Format;

		uint32_t m_mipLevels = 1;

		VkImage m_Image = VK_NULL_HANDLE;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;
		VkSampler m_Sampler = VK_NULL_HANDLE;
		VkImageView m_View = VK_NULL_HANDLE;
	};
}