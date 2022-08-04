#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "WindowHandler.h"

class Window : public WindowHandler
{
public:
	Window(const int width, const int height, const std::string& title);
	~Window();
	
	GLFWwindow* getGLFWWindow() const noexcept;
	WindowSize getFramebufferSize() const override;
	void waitEvents() const override;
	void open(std::function<void(WindowHandler&)> drawFrame);

private:
	void setGlfwWindowHints() const;
	void createGlfwWindow();
	
	const int width;
	const int height;
	const std::string title;
	GLFWwindow* glfwWindow;
};