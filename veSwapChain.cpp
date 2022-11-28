#include "veSwapChain.hpp"


vengin::veSwapChain::veSwapChain()
{
}

//表面格式(颜色，深度)
VkSurfaceFormatKHR vengin::veSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}
//呈现模式可以说是交换链中最重要的设置。它决定了什么条件下图像才会显示到屏幕。
VkPresentModeKHR vengin::veSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}
//交换范围是交换链中图像的分辨率，它几乎总是和我们要显示图像的窗口的分辨率相同。VkSurfaceCapabilitiesKHR结构体定义了可用的分辨率范围。
//Vulkan通过currentExtent成员变量来告知适合我们窗口的交换范围。一些窗口系统会使用一个特殊值，uint32_t变量类型的最大值，表示允许我们自己选择对于窗口最合适的交换范围，
//但我们选择的交换范围需要在minImageExtent与maxImageExtent的范围内。
VkExtent2D vengin::veSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { WIDTH, HEIGHT };

		actualExtent.width = std::max(capabilities.minImageExtent.width,
			std::min(capabilities.maxImageExtent.width,
				actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height,
			std::min(capabilities.maxImageExtent.height,
				actualExtent.height));

		return actualExtent;
	}
}

void vengin::veSwapChain::getSwapChainImages(VkDevice& device)
{
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);//获取数量
	swapChainImages.resize(imageCount);//设置交换链图像容器大小
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());//获取交换链图像
}
