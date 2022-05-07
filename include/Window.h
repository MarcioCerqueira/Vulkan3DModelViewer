#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height);
	~Window();
	
	void open();

private:
	void setGlfwWindowHints() const;
	void createGlfwWindow();
	
	int width;
	int height;
	GLFWwindow* glfwWindow;
};