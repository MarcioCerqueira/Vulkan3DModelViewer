#include "Shader.h"

Shader::Shader(const std::string& filename, const vk::Device& vulkanLogicalDevice, const vk::ShaderStageFlagBits stage) : stage(stage), vulkanLogicalDevice(vulkanLogicalDevice)
{
	std::vector<char> shaderCode{ readFile(filename) };
	createShaderModule(shaderCode, vulkanLogicalDevice);
	std::cout << "Shader file located at " << filename << " has been loaded" << std::endl;
}

Shader::~Shader()
{
	vulkanLogicalDevice.destroyShaderModule(vulkanShaderModule);
}

const std::vector<char> Shader::readFile(const std::string& filename) const
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	ExceptionChecker::throwExceptionIfFileCouldNotBeOpened(file, filename.c_str());
	size_t fileSize{ static_cast<size_t>(file.tellg()) };
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

void Shader::createShaderModule(const std::vector<char>& shaderCode, const vk::Device& vulkanLogicalDevice)
{

	vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
		.codeSize = shaderCode.size(),
		.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data())
	};
	vulkanShaderModule = vulkanLogicalDevice.createShaderModule(shaderModuleCreateInfo);
}

const vk::PipelineShaderStageCreateInfo Shader::buildPipelineShaderStageCreateInfo() const
{
	return vk::PipelineShaderStageCreateInfo{
		.stage = stage,
		.module = vulkanShaderModule,
		.pName = "main"
	};
}
