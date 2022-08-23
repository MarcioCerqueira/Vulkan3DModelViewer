#include "Window.h"

Window::Window(const WindowSize& windowSize, const std::string& title, CameraHandler& cameraHandler) : width(windowSize.width), height(windowSize.height), title(title), cameraHandler(cameraHandler)
{
	glfwInit();
	setGlfwWindowHints();
	createGlfwWindow();
	setCallbacks();
	initializeMouseInfo();
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

void Window::setCallbacks()
{
	glfwSetWindowUserPointer(glfwWindow, this);
	glfwSetCursorPosCallback(glfwWindow, mouseCallback);
	glfwSetScrollCallback(glfwWindow, scrollCallback);
}

void Window::initializeMouseInfo()
{
	mouseInfo.firstInteraction = true;
	mouseInfo.lastX = static_cast<float>(width) / 2.0f;
	mouseInfo.lastY = static_cast<float>(height) / 2.0f;
}

GLFWwindow* Window::getGLFWWindow() const
{
	return glfwWindow;
}

WindowSize Window::getFramebufferSize() const
{
	WindowSize framebufferSize;
	glfwGetFramebufferSize(glfwWindow, &framebufferSize.width, &framebufferSize.height);
	return framebufferSize;
}

void Window::waitEvents() const
{
	glfwWaitEvents();
}

void Window::open(std::function<void(WindowHandler&, CameraHandler&)> drawFrame)
{
	while(!glfwWindowShouldClose(glfwWindow)) 
	{
		glfwPollEvents();
		updateFrameTime();
		processKeyboard();
		drawFrame(*this, cameraHandler);	
	}
}

void Window::processKeyboard()
{
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glfwWindow, true);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraHandler.processKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraHandler.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraHandler.processKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraHandler.processKeyboard(CameraMovement::RIGHT, deltaTime);
	}
}

void Window::updateFrameTime()
{
	const float currentFrameTime{ static_cast<float>(glfwGetTime()) };
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
}

void Window::mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->mouseCallback(xposIn, yposIn);
}

void Window::scrollCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->scrollCallback(xposIn, yposIn);
}

void Window::mouseCallback(double xposIn, double yposIn)
{
	float xpos{ static_cast<float>(xposIn) };
	float ypos{ static_cast<float>(yposIn) };
    if (mouseInfo.firstInteraction)
    {
		updateMouseInfo(xpos, ypos);
    }
	float xoffset{ xpos - mouseInfo.lastX };
	float yoffset{ mouseInfo.lastY - ypos };
	cameraHandler.processMouseMovement(xoffset, yoffset, true);
	updateMouseInfo(xpos, ypos);
}

void Window::updateMouseInfo(float lastX, float lastY)
{
	mouseInfo.firstInteraction = false;
	mouseInfo.lastX = lastX;
	mouseInfo.lastY = lastY;
}

void Window::scrollCallback(double xoffset, double yoffset)
{
	cameraHandler.processMouseScroll(yoffset);
}
