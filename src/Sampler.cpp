#include "Sampler.h"

Sampler::Sampler(const vk::Device& vulkanLogicalDevice, const vk::PhysicalDeviceProperties& physicalDeviceProperties) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::SamplerCreateInfo samplerCreateInfo{ buildSamplerCreateInfo(physicalDeviceProperties) };
	vulkanSampler = vulkanLogicalDevice.createSampler(samplerCreateInfo);
}

Sampler::~Sampler()
{
	vulkanLogicalDevice.destroySampler(vulkanSampler);
}

const vk::SamplerCreateInfo Sampler::buildSamplerCreateInfo(const vk::PhysicalDeviceProperties& physicalDeviceProperties) const
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
		.maxLod = 0.0f,
		.borderColor = vk::BorderColor::eIntOpaqueBlack,
		.unnormalizedCoordinates = vk::Bool32{false}
	};
}

const vk::Sampler Sampler::getVulkanSampler() const
{
	return vulkanSampler;
}