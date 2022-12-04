#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#define WIDTH 800
#define HEIGHT 600
namespace vengin {
	class veWindow
	{
	public:
		//veWindow();
		veWindow(int width, int height, const char* title) ;
		~veWindow();

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
			auto app = reinterpret_cast<veWindow*>(glfwGetWindowUserPointer(window));
			app->framebufferResized = true;
		}

		veWindow(const veWindow&) = delete;
		veWindow &operator=(const veWindow&) = delete;

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);;
		bool shouldClose() { return glfwWindowShouldClose(window); }
		void initWindow();
		GLFWwindow* window;
		bool framebufferResized = false;
	private:
		//create function
		


		//obj|handle
		


		int width;
		int height;
		const char* title;
	};
}
