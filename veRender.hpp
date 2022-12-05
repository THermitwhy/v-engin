#pragma once
#include "veDevice.hpp"
#include "veSwapChain.hpp"
#include "vertexes.hpp"
namespace vengin {
	class veRender {
	public:
		veRender(veDevice& vedevice);
		void createSwapChain();
		void recreateSwapChain();
		void createCommandPool();
		void createCommandBuffers();
		void createVertexBuffer();//创建顶点缓冲
		void clean();
		void cleanUpVeSwapChain();
		void updateVeSwapChain();


		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);//寻找适合的内存类型
		veDevice& vedevice;
		veSwapChain *veswapchain;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
	};
}
