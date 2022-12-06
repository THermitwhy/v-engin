#pragma once
#include "veDevice.hpp"
#include "veSwapChain.hpp"
#include "vertexes.hpp"

#include <stb_image.h>
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

		void createTextureImage();//创建纹理贴图
		VkImageView textureImageView;//创建纹理贴图对象视图
		VkImage textureImage;//用于存储纹理的图像对象
		VkSampler textureSampler;//采样器对象
		void createTextureImageView();//用于创建图像对象的视图
		void createTextureSampler();//创建图像的采样器
		VkDeviceMemory textureImageMemory;//为图像对象开辟的内存
		void createImage(uint32_t width, uint32_t height, VkFormat
			format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image,
			VkDeviceMemory& imageMemory);//用于创建图像对象简化操作
		    VkImageView createImageView(VkImage image, VkFormat format);//简化图像视图对象的创建
		void transitionImageLayout(VkImage image, VkFormat format,
			VkImageLayout oldLayout, VkImageLayout newLayout);//用于图像的布局变换
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t
			width, uint32_t height);

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

		//辅助指令函数
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	};
}
