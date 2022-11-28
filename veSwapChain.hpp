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
		//obj
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;

		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
	};
}
