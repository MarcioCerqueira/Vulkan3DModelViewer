#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "WindowSize.h"

class Window
{
public:
	Window(const int width, const int height, const std::string& title);
	~Window();
	
	GLFWwindow* getGLFWWindow() const noexcept;
	WindowSize getFramebufferSize() const;
	void waitEvents() const;
	void open(std::function<void(std::function<WindowSize()>, std::function<void()>)> drawFrame);

private:
	void setGlfwWindowHints() const;
	void createGlfwWindow();
	
	const int width;
	const int height;
	const std::string title;
	GLFWwindow* glfwWindow;
};