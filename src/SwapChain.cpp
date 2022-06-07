#include "SwapChain.h"

SwapChain::SwapChain(const SwapChainCreateInfo& swapChainCreateInfo) : swapChainCreateInfo(swapChainCreateInfo)
{
    vk::SurfaceCapabilitiesKHR capabilities{ swapChainCreateInfo.vulkanPhysicalDevice.getSurfaceCapabilitiesKHR(swapChainCreateInfo.vulkanWindowSurface) };
    const std::vector<vk::SurfaceFormatKHR> availableFormats{ swapChainCreateInfo.vulkanPhysicalDevice.getSurfaceFormatsKHR(swapChainCreateInfo.vulkanWindowSurface) };
    std::vector<vk::PresentModeKHR> availablePresentModes{ swapChainCreateInfo.vulkanPhysicalDevice.getSurfacePresentModesKHR(swapChainCreateInfo.vulkanWindowSurface) };

    chooseSwapSurfaceFormat(availableFormats);
    chooseSwapPresentMode(availablePresentModes);
    chooseSwapExtent(capabilities, swapChainCreateInfo.framebufferSize);
    buildVulkanSwapChain(swapChainCreateInfo, capabilities, estimateImageCount(capabilities));
    buildSwapChainImageViews(swapChainCreateInfo);
}

void SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    surfaceFormat = availableFormats[0];
    for (const auto& availableFormat : availableFormats) 
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) 
        {
            surfaceFormat = availableFormat;
            return;
        }
    }
}

void SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    presentMode = vk::PresentModeKHR::eFifo;
    for (const auto& availablePresentMode : availablePresentModes) 
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) 
        {
            presentMode = availablePresentMode;
            return;
        }
    }
}

void SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowSize& framebufferSize)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        extent = capabilities.currentExtent;
    }
    else
    {
        extent = vk::Extent2D{ static_cast<uint32_t>(framebufferSize.width), static_cast<uint32_t>(framebufferSize.height) };
        extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
}

uint32_t SwapChain::estimateImageCount(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    uint32_t imageCount{ capabilities.minImageCount + 1 };
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) 
    {
        imageCount = capabilities.maxImageCount;
    }
    return imageCount;
}

bool SwapChain::isValid(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface)
{
    const std::vector<vk::SurfaceFormatKHR> availableFormats{ vulkanPhysicalDevice.getSurfaceFormatsKHR(vulkanWindowSurface) };
    const std::vector<vk::PresentModeKHR> availablePresentModes{ vulkanPhysicalDevice.getSurfacePresentModesKHR(vulkanWindowSurface) };
    return !availableFormats.empty() && !availablePresentModes.empty();
}

void SwapChain::buildVulkanSwapChain(const SwapChainCreateInfo& swapChainCreateInfo, const vk::SurfaceCapabilitiesKHR& capabilities, const uint32_t imageCount)
{
    std::optional<uint32_t> graphicsFamilyIndex{ swapChainCreateInfo.queueFamilyIndices.getGraphicsFamilyIndex() };
    std::optional<uint32_t> presentFamilyIndex{ swapChainCreateInfo.queueFamilyIndices.getPresentFamilyIndex() };
    uint32_t queueFamilyIndices[] = { graphicsFamilyIndex.value(), presentFamilyIndex.value() };
    vk::SwapchainCreateInfoKHR swapChainCreateInfoKHR{
        .sType = vk::StructureType::eSwapchainCreateInfoKHR,
        .flags = {},
        .surface = swapChainCreateInfo.vulkanWindowSurface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = (graphicsFamilyIndex != presentFamilyIndex) ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
        .queueFamilyIndexCount = (graphicsFamilyIndex != presentFamilyIndex) ? static_cast<uint32_t>(2) : static_cast<uint32_t>(0),
        .pQueueFamilyIndices = (graphicsFamilyIndex != presentFamilyIndex) ? queueFamilyIndices : nullptr,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = presentMode
    };
    vulkanSwapChain = swapChainCreateInfo.vulkanLogicalDevice.createSwapchainKHR(swapChainCreateInfoKHR);
    images = swapChainCreateInfo.vulkanLogicalDevice.getSwapchainImagesKHR(vulkanSwapChain);
}


void SwapChain::buildSwapChainImageViews(const SwapChainCreateInfo& swapChainCreateInfo)
{
    imageViews.resize(images.size());
    for (int imageIndex = 0; imageIndex < images.size(); ++imageIndex)
    {
        vk::ImageViewCreateInfo imageViewCreateInfo{ buildImageViewCreateInfo(imageIndex) };
        imageViews[imageIndex] = swapChainCreateInfo.vulkanLogicalDevice.createImageView(imageViewCreateInfo);
    }
}

vk::ImageViewCreateInfo SwapChain::buildImageViewCreateInfo(const int imageIndex) const
{
    return vk::ImageViewCreateInfo{
        .sType = vk::StructureType::eImageViewCreateInfo,
        .image = images[imageIndex],
        .viewType = vk::ImageViewType::e2D,
        .format = surfaceFormat.format,
        .components = vk::ComponentSwizzle::eIdentity,
        .subresourceRange = createImageSubresourceRange()
    };
}
vk::ImageSubresourceRange SwapChain::createImageSubresourceRange() const
{
    return vk::ImageSubresourceRange{
        .aspectMask = vk::ImageAspectFlagBits::eColor,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    };
}

vk::Extent2D SwapChain::getExtent() const noexcept
{
    return extent;
}

vk::SurfaceFormatKHR SwapChain::getSurfaceFormat() const noexcept
{
    return surfaceFormat;
}
