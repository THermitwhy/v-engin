#pragma once
#include "veWindow.hpp"
namespace vengin {
	class veSwapChain {
	public:
		veSwapChain();

		//func
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>
			& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>
			availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void getSwapChainImages(VkDevice& device);
		void createImageViews(VkDevice &device);
		void cleanImageViews(VkDevice& device);
		//obj
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;//������ͼ��
		std::vector<VkImageView> swapChainImageViews;//������ͼ����ͼ

		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
	};
}
