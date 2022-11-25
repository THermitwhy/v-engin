#pragma once
#include "veWindow.hpp"
#include "veDevice.hpp"
//#include "vePipeline.hpp"
//#include "veRender.hpp"
namespace vengin {
	class applic
	{
	public:
		applic();
		~applic();
		//禁止拷贝构造
		applic(const applic&) = delete;
		applic& operator=(const applic&) = delete;
		//void setPipelineConfig(pipelineConfigInfo& config);
		//void createPipelineLayout();
		void run();
	private:
		veWindow vWindow{800,600,"hello vulkan"};
		veDevice vDevice{ vWindow };
		//veRender render;
		//vePipeline vPipeline;//写函数定义吧
		//VkPipelineLayout pipelineLayout;
		//beginFrame->beginRender->endRender->endFrame这样子   
		
	};
}
