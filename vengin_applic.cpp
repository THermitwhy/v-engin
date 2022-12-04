#include "vengin_applic.hpp"

namespace vengin {
	applic::applic()
	{

		//vWindow.initWindow();
		//veDevice device(vWindow);
		createSyncObjects();
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
		vkDeviceWaitIdle(vDevice.getDevice());
		clean();
	}
	void applic::draw()
	{
		vkWaitForFences(vDevice.getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(vDevice.getDevice(), 1, &inFlightFences[currentFrame]);
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(vDevice.getDevice(), vRender.veswapchain->swapChain, std::numeric_limits<uint64_t>::max(),
			imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vRender.commandBuffers[imageIndex];
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[imageIndex] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;




		if (result == VK_ERROR_OUT_OF_DATE_KHR || 
			vWindow.framebufferResized) {
			vWindow.framebufferResized = false;
			//vkResetFences(vDevice.getDevice(), 1, &inFlightFences[currentFrame]);
			vRender.updateVeSwapChain();
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(vDevice.getDevice(), renderFinishedSemaphores[i], nullptr);
				vkDestroySemaphore(vDevice.getDevice(), imageAvailableSemaphores[i], nullptr);
				vkDestroyFence(vDevice.getDevice(), inFlightFences[i], nullptr);
			}
			createSyncObjects();
			//vkResetFences(vDevice.getDevice(), 1, &inFlightFences[currentFrame]);
			/*if (vkQueueSubmit(vDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
				throw std::runtime_error("failed to submit draw command buffer!");
			}*/
			

			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
		}


		
		//vkAcquireNextImageKHR(vDevice.getDevice(), vRender.veswapchain->swapChain, std::numeric_limits<uint64_t>::max(),
		//	imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (vkQueueSubmit(vDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
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
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}
	void applic::createSyncObjects() {
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(vDevice.getDevice(), &semaphoreInfo, nullptr,
				&imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(vDevice.getDevice(), &semaphoreInfo, nullptr,
					&renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(vDevice.getDevice(), &fenceInfo, nullptr,
					&inFlightFences[i]) != VK_SUCCESS) {

				throw std::runtime_error("failed to create synchronization objects for a frame!");
			}
		}
	}
	void applic::clean()
	{
		vRender.clean();
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(vDevice.getDevice(), renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(vDevice.getDevice(), imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(vDevice.getDevice(), inFlightFences[i], nullptr);
		}
		
	}
}