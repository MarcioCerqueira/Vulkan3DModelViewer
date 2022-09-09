#include "SwapChain.h"

SwapChain::SwapChain(const SwapChainCreateInfo& swapChainCreateInfo) : swapChainCreateInfo(swapChainCreateInfo)
{
    const vk::PhysicalDevice vulkanPhysicalDevice{ swapChainCreateInfo.physicalDeviceProperties.getVulkanPhysicalDevice() };
    vk::SurfaceCapabilitiesKHR capabilities{ vulkanPhysicalDevice.getSurfaceCapabilitiesKHR(swapChainCreateInfo.vulkanWindowSurface) };
    const std::vector<vk::SurfaceFormatKHR> availableFormats{ vulkanPhysicalDevice.getSurfaceFormatsKHR(swapChainCreateInfo.vulkanWindowSurface) };
    std::vector<vk::PresentModeKHR> availablePresentModes{ vulkanPhysicalDevice.getSurfacePresentModesKHR(swapChainCreateInfo.vulkanWindowSurface) };
    chooseSwapSurfaceFormat(availableFormats);
    chooseSwapPresentMode(availablePresentModes);
    chooseSwapExtent(capabilities, swapChainCreateInfo.framebufferSize);
    buildVulkanSwapChain(swapChainCreateInfo, capabilities, estimateImageCount(capabilities));
    buildSwapChainImageViews(swapChainCreateInfo);
    buildDepthImage();
    buildColorImage();
}

SwapChain::~SwapChain()
{
    cleanup();
}

void SwapChain::cleanup()
{
    for (int imageIndex = 0; imageIndex < imageViews.size(); imageIndex++)
    {
        framebuffers[imageIndex].reset();
        imageViews[imageIndex].reset();
    }
    depthImage.reset();
    colorImage.reset();
    swapChainCreateInfo.vulkanLogicalDevice.destroySwapchainKHR(vulkanSwapChain);
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
        const ImageViewInfo imageViewInfo{
            .vulkanLogicalDevice = swapChainCreateInfo.vulkanLogicalDevice,
            .image = images[imageIndex],
            .format = surfaceFormat.format,
            .aspectMask = vk::ImageAspectFlagBits::eColor,
            .mipLevels = 1
        };
        imageViews[imageIndex] = std::make_unique<ImageView>(imageViewInfo);
    }
}

void SwapChain::buildDepthImage()
{
    const ImageInfo depthImageInfo{ buildDepthImageInfo() };
    depthImage = std::make_unique<Image>(depthImageInfo);
    depthImage->createImageView(vk::ImageAspectFlagBits::eDepth);
}

ImageInfo SwapChain::buildDepthImageInfo() const
{
    return ImageInfo{
        .vulkanLogicalDevice = swapChainCreateInfo.vulkanLogicalDevice,
        .physicalDeviceProperties = swapChainCreateInfo.physicalDeviceProperties,
        .width = static_cast<int>(extent.width),
        .height = static_cast<int>(extent.height),
        .mipLevels = 1,
        .sampleCount = swapChainCreateInfo.physicalDeviceProperties.getMaxUsableSampleCount(),
        .format = swapChainCreateInfo.depthImageFormat,
        .usageFlags = vk::ImageUsageFlagBits::eDepthStencilAttachment
    };
}

void SwapChain::buildColorImage()
{
    const ImageInfo colorImageInfo{ buildColorImageInfo() };
    colorImage = std::make_unique<Image>(colorImageInfo);
    colorImage->createImageView(vk::ImageAspectFlagBits::eColor);
}

ImageInfo SwapChain::buildColorImageInfo() const
{
    return ImageInfo{
        .vulkanLogicalDevice = swapChainCreateInfo.vulkanLogicalDevice,
        .physicalDeviceProperties = swapChainCreateInfo.physicalDeviceProperties,
        .width = static_cast<int>(extent.width),
        .height = static_cast<int>(extent.height),
        .mipLevels = 1,
        .sampleCount = swapChainCreateInfo.physicalDeviceProperties.getMaxUsableSampleCount(),
        .format = surfaceFormat.format,
        .usageFlags = vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment
    };
}
void SwapChain::buildFramebuffers(const vk::Device& vulkanLogicalDevice, const vk::RenderPass& vulkanRenderPass)
{
    framebuffers.resize(imageViews.size());
    for (int framebufferIndex = 0; framebufferIndex < framebuffers.size(); framebufferIndex++)
    {
        const std::vector<vk::ImageView> attachments = { colorImage->getVulkanImageView(), depthImage->getVulkanImageView(), imageViews[framebufferIndex]->getVulkanImageView() };
        framebuffers[framebufferIndex] = std::make_unique<Framebuffer>(vulkanLogicalDevice, vulkanRenderPass, attachments, extent);
    }
}

vk::Result SwapChain::acquireNextImage(vk::Semaphore& imageAvailable, const vk::RenderPass& vulkanRenderPass, uint32_t& imageIndex)
{
    constexpr uint64_t timeout{ std::numeric_limits<uint64_t>::max() };
    return swapChainCreateInfo.vulkanLogicalDevice.acquireNextImageKHR(vulkanSwapChain, timeout, imageAvailable, nullptr, &imageIndex);
}

void SwapChain::recreateIfResultIsOutOfDateOrSuboptimalKHR(vk::Result& result, const vk::RenderPass& vulkanRenderPass, WindowHandler& windowHandler)
{
    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
    {
        waitValidFramebufferSize(windowHandler);
        swapChainCreateInfo.vulkanLogicalDevice.waitIdle();
        cleanup();
        buildVulkanSwapChain(swapChainCreateInfo, swapChainCreateInfo.physicalDeviceProperties.getVulkanPhysicalDevice().getSurfaceCapabilitiesKHR(swapChainCreateInfo.vulkanWindowSurface), static_cast<const uint32_t>(images.size()));
        buildSwapChainImageViews(swapChainCreateInfo);
        buildDepthImage();
        buildColorImage();
        buildFramebuffers(swapChainCreateInfo.vulkanLogicalDevice, vulkanRenderPass);
        result = vk::Result::eSuccess;
    }
}

void SwapChain::waitValidFramebufferSize(WindowHandler& windowHandler)
{
    WindowSize framebufferSize = windowHandler.getFramebufferSize();
    while (framebufferSize.width == 0 || framebufferSize.height == 0)
    {
        framebufferSize = windowHandler.getFramebufferSize();
        windowHandler.waitEvents();
    }
    extent.width = framebufferSize.width;
    extent.height = framebufferSize.height;
}

const vk::Extent2D& SwapChain::getExtent() const
{
    return extent;
}

const vk::Framebuffer& SwapChain::getVulkanFramebuffer(const int framebufferIndex) const
{
    return framebuffers[framebufferIndex]->getVulkanFramebuffer();
}

const vk::SurfaceFormatKHR& SwapChain::getSurfaceFormat() const
{
    return surfaceFormat;
}

const vk::SwapchainKHR& SwapChain::getVulkanSwapChain() const
{
    return vulkanSwapChain;
}