#include "veSwapChain.hpp"


vengin::veSwapChain::veSwapChain()
{
}

//�����ʽ(��ɫ�����)
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
//����ģʽ����˵�ǽ�����������Ҫ�����á���������ʲô������ͼ��Ż���ʾ����Ļ��
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
//������Χ�ǽ�������ͼ��ķֱ��ʣ����������Ǻ�����Ҫ��ʾͼ��Ĵ��ڵķֱ�����ͬ��VkSurfaceCapabilitiesKHR�ṹ�嶨���˿��õķֱ��ʷ�Χ��
//Vulkanͨ��currentExtent��Ա��������֪�ʺ����Ǵ��ڵĽ�����Χ��һЩ����ϵͳ��ʹ��һ������ֵ��uint32_t�������͵����ֵ����ʾ���������Լ�ѡ����ڴ�������ʵĽ�����Χ��
//������ѡ��Ľ�����Χ��Ҫ��minImageExtent��maxImageExtent�ķ�Χ�ڡ�
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
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);//��ȡ����
	swapChainImages.resize(imageCount);//���ý�����ͼ��������С
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());//��ȡ������ͼ��
}
