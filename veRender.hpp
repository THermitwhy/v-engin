#pragma once
#include "veDevice.hpp"
#include "veSwapChain.hpp"
namespace vengin {
	class veRender {
	public:
		veRender(veDevice& vedevice);
		void createCommandPool();
		void createCommandBuffers();
		void clean();
		veDevice& vedevice;
		veSwapChain *veswapchain;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}
