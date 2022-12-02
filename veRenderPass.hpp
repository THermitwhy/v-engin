#pragma once
#include "veWindow.hpp"

namespace vengin {
	class veRenderPass {
	public:
		veRenderPass(VkDevice& device,VkFormat swapChainImageFormat);
		void createRenderPass();
		void cleanRenderPass();
		VkFormat swapChainImageFormat;
		VkRenderPass renderPass;
		VkDevice& device;
	};
}
