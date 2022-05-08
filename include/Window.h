#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	Window(int width, int height, const std::string& title);
	~Window();
	
	void open();

private:
	void setGlfwWindowHints() const;
	void createGlfwWindow();
	
	int width;
	int height;
	const std::string title;
	GLFWwindow* glfwWindow;
};