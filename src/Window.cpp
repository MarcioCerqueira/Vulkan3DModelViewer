#include "Window.h"

Window::Window(const int width, const int height, const std::string& title) : width(width), height(height), title(title)
{
	glfwInit();
	setGlfwWindowHints();
	createGlfwWindow();
}

Window::~Window()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

void Window::setGlfwWindowHints() const
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void Window::createGlfwWindow()
{
	glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

GLFWwindow* Window::getGLFWWindow() const noexcept
{
	return glfwWindow;
}

WindowSize Window::getFramebufferSize() const
{
	WindowSize framebufferSize;
	glfwGetFramebufferSize(glfwWindow, &framebufferSize.width, &framebufferSize.height);
	return framebufferSize;
}

void Window::open(std::function<void()> drawFrame)
{
	while(!glfwWindowShouldClose(glfwWindow)) 
	{
		glfwPollEvents();
		drawFrame();
	}
}