#include "vengin_applic.hpp"

namespace vengin {
	applic::applic()
	{
		//vWindow.initWindow();
		//veDevice device(vWindow);
		createSemaphores();
	}

	applic::~applic()
	{
	}
	//void applic::createPipelineLayout() {
	//	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	//	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	//	pipelineLayoutInfo.setLayoutCount = 0;
	//	pipelineLayoutInfo.pSetLayouts = nullptr;
	//	pipelineLayoutInfo.pushConstantRangeCount = 0;
	//	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	//	if (vkCreatePipelineLayout(vDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
	//		throw std::runtime_error("fail to create pipelineLayout!");
	//	}

	//}
	//void applic::setPipelineConfig(pipelineConfigInfo& config)
	//{
	//	
	//}

	void applic::run()
	{
		while (!vWindow.shouldClose()) {
			glfwPollEvents();
			draw();
		}
	}
	void applic::draw()
	{
		uint32_t imageIndex;
		vkAcquireNextImageKHR(vDevice.getDevice(), vRender.veswapchain->swapChain, std::numeric_limits<uint64_t>::max(),
			imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vRender.commandBuffers[imageIndex];
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		if (vkQueueSubmit(vDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { vRender.veswapchain->swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional
		vkQueuePresentKHR(vDevice.getPresentQueue(), &presentInfo);
	}
	void applic::createSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(vDevice.getDevice(), &semaphoreInfo, nullptr,
			&imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(vDevice.getDevice(), &semaphoreInfo, nullptr,
				&renderFinishedSemaphore) != VK_SUCCESS) {

			throw std::runtime_error("failed to create semaphores!");
		}
	}
}