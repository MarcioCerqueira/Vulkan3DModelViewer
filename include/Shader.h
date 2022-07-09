#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "ExceptionChecker.h"

class Shader
{
public:
	Shader(const std::string& filename, const vk::Device& vulkanLogicalDevice, const vk::ShaderStageFlagBits stage);
	~Shader();

	const vk::PipelineShaderStageCreateInfo buildPipelineShaderStageCreateInfo() const;

private:
	const std::vector<char> readFile(const std::string& filename) const;
	void createShaderModule(const std::vector<char>& shaderCode, const vk::Device& vulkanLogicalDevice);

	const vk::Device vulkanLogicalDevice;
	vk::ShaderModule vulkanShaderModule;
	const vk::ShaderStageFlagBits stage;

};