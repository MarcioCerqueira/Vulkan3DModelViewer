#include "Window.h"

Window::Window(int width, int height) : width(width), height(height)
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
	glfwWindow = glfwCreateWindow(width, height, "Ambient Occlusion Application", nullptr, nullptr);
}

void Window::open()
{
	while(!glfwWindowShouldClose(glfwWindow)) 
	{
		glfwPollEvents();
	}
}