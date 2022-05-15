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

void Window::open()
{
	while(!glfwWindowShouldClose(glfwWindow)) 
	{
		glfwPollEvents();
	}
}