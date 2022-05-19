#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	Window(const int width, const int height, const std::string& title);
	~Window();
	
	GLFWwindow* getGLFWWindow() const noexcept;
	void open();

private:
	void setGlfwWindowHints() const;
	void createGlfwWindow();
	
	const int width;
	const int height;
	const std::string title;
	GLFWwindow* glfwWindow;
};