#include "Sampler.h"

Sampler::Sampler(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDeviceProperties& physicalDeviceProperties, const uint32_t mipLevels) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::SamplerCreateInfo samplerCreateInfo{ buildSamplerCreateInfo(physicalDeviceProperties, mipLevels) };
	vulkanSampler = vulkanLogicalDevice.createSampler(samplerCreateInfo);
}

Sampler::~Sampler()
{
	vulkanLogicalDevice.destroySampler(vulkanSampler);
}

vk::SamplerCreateInfo Sampler::buildSamplerCreateInfo(const vk::PhysicalDeviceProperties& physicalDeviceProperties, const uint32_t mipLevels) const
{
	return vk::SamplerCreateInfo{
		.magFilter = vk::Filter::eLinear,
		.minFilter = vk::Filter::eLinear,
		.mipmapMode = vk::SamplerMipmapMode::eLinear,
		.addressModeU = vk::SamplerAddressMode::eRepeat,
		.addressModeV = vk::SamplerAddressMode::eRepeat,
		.addressModeW = vk::SamplerAddressMode::eRepeat,
		.mipLodBias = 0.0f,
		.anisotropyEnable = vk::Bool32{true},
		.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,
		.compareEnable = vk::Bool32{false},
		.compareOp = vk::CompareOp::eAlways,
		.minLod = 0.0f,
		.maxLod = static_cast<float>(mipLevels),
		.borderColor = vk::BorderColor::eIntOpaqueBlack,
		.unnormalizedCoordinates = vk::Bool32{false}
	};
}

const vk::Sampler& Sampler::getVulkanSampler() const
{
	return vulkanSampler;
}