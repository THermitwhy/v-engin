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
		void createFrameBuffers();
		void cleanImageViews(VkDevice& device);
		//obj
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;//½»»»Á´Í¼Ïñ
		std::vector<VkImageView> swapChainImageViews;//½»»»Á´Í¼ÏñÊÓÍ¼
		std::vector<VkFramebuffer> swapChainFramebuffers;//Ö¡»º³åÈÝÆ÷
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
	};
}
