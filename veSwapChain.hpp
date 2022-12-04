#pragma once
#include "veDevice.hpp"
#include "vePipeline.hpp"
namespace vengin {
	class veSwapChain {
	public:
		veSwapChain(veDevice& vedevice);
		~veSwapChain();
		void init();
		void clean();
		//func
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>
			& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>
			availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		void createSwapChain();
		void getSwapChainImages();
		void createImageViews();
		void createRenderPass();
		void createPipeline();
		void createFrameBuffers();
		//void createCommandBuffers();
		void cleanImageViews();
		//obj
		VkSwapchainKHR swapChain;
		veDevice& vedevice;
		//GLFWwindow* window;
		//vePipeline vepipeline;
		std::vector<VkImage> swapChainImages;//½»»»Á´Í¼Ïñ
		std::vector<VkImageView> swapChainImageViews;//½»»»Á´Í¼ÏñÊÓÍ¼
		VkRenderPass renderPass;//äÖÈ¾¸½×Å
		std::vector<VkFramebuffer> swapChainFramebuffers;//Ö¡»º³åÈÝÆ÷
		//std::vector<VkCommandBuffer> commandBuffers;
		vePipeline *vepipeline;/////
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
	};
}
