#pragma once
#include "veDevice.hpp"
#include "veSwapChain.hpp"
namespace vengin {
	class veRender {
	public:
		veRender(veDevice& vedevice);
		void createSwapChain();
		void recreateSwapChain();
		void createCommandPool();
		void createCommandBuffers();
		void clean();
		void cleanUpVeSwapChain();
		void updateVeSwapChain();
		veDevice& vedevice;
		veSwapChain *veswapchain;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}
