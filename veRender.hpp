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
		
		void clean();
		void cleanUpVeSwapChain();
		void updateVeSwapChain();


		void createVertexBuffer();//创建顶点缓冲
		void createIndexBuffer();//创建索引缓冲
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);//创建缓冲区
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);//将数据从源缓冲区复制到目标缓冲区

		void createUniformBuffer();
		void updateUniformBuffer(uint32_t currentImage);

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);//寻找适合的内存类型
		veDevice& vedevice;
		veSwapChain *veswapchain;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;


		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		void createDescriptorPool();
		void createDescriptorSets();
	};
}
