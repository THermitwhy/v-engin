#include "vengin_applic.hpp"

namespace vengin {
	applic::applic()
	{
		//vWindow.initWindow();
		//veDevice device(vWindow);
		
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
		}
	}
}