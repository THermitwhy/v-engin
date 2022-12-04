#include "veRender.hpp"
namespace vengin {
	veRender::veRender(veDevice& vedevice):vedevice(vedevice)
	{
		createSwapChain();
		createCommandPool();
		createCommandBuffers();
	}
	void veRender::createSwapChain()
	{
		veswapchain = new veSwapChain(vedevice);
	}
	void veRender::createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = vedevice.findPhysicalFamilies();

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = 0; // Optional

		if (vkCreateCommandPool(vedevice.getDevice(), &poolInfo, nullptr,
			&commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
	}

	void veRender::createCommandBuffers()
	{
		commandBuffers.resize(veswapchain->swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(vedevice.getDevice(), &allocInfo,
			commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = nullptr; // Optional

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}
			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = veswapchain->renderPass;
			renderPassInfo.framebuffer = veswapchain->swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = veswapchain->swapChainExtent;

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, veswapchain->vepipeline->graphicsPipeline);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		
	}

	void veRender::clean()
	{
		vkDestroyCommandPool(vedevice.getDevice(), commandPool, nullptr);
		delete veswapchain;
	}

	void veRender::cleanUpVeSwapChain()
	{
	}

	void veRender::updateVeSwapChain()
	{
		vkDeviceWaitIdle(vedevice.getDevice());
		vkFreeCommandBuffers(vedevice.getDevice(), commandPool,
			static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		veswapchain->clean();
		veswapchain->init();
		createCommandBuffers();


	}

	
}