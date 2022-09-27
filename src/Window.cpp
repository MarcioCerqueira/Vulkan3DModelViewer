#include "Window.h"

Window::Window(const WindowSize& windowSize, const std::string& title, CameraHandler& cameraHandler) : width(windowSize.width), height(windowSize.height), title(title), cameraHandler(cameraHandler)
{
	glfwInit();
	setGlfwWindowHint();
	createGlfwWindow();
	setCallbacks();
	initializeCameraHandler();
}

Window::~Window()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

void Window::setGlfwWindowHint() const
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void Window::createGlfwWindow()
{
	glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

void Window::setCallbacks()
{
	glfwSetWindowUserPointer(glfwWindow, this);
	glfwSetCursorPosCallback(glfwWindow, cursorPosCallback);
	glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallback);
	glfwSetScrollCallback(glfwWindow, scrollCallback);
	glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);
}

void Window::initializeCameraHandler()
{
	cameraHandler.setWindowSize(glm::ivec2(width, height));
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

bool Window::showTexture() const
{
	return textureShouldBeVisible;
}

void Window::open(std::function<void(WindowHandler&, CameraHandler&, bool)> drawFrame)
{
	while(!glfwWindowShouldClose(glfwWindow)) 
	{
		glfwPollEvents();
		updateFrameTime();
		processKeyboard();
		drawFrame(*this, cameraHandler, framebufferResized);
		if (framebufferResized)
		{
			setFramebufferResized(false);
		}
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
		cameraHandler.processKeyboard(CameraMovement::Forward, deltaTime);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraHandler.processKeyboard(CameraMovement::Backward, deltaTime);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraHandler.processKeyboard(CameraMovement::Left, deltaTime);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraHandler.processKeyboard(CameraMovement::Right, deltaTime);
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_C) == GLFW_PRESS)
	{
		cameraHandler.printCameraData();
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_T) == GLFW_PRESS)
	{
		textureShouldBeVisible = true;
	}
	if (glfwGetKey(glfwWindow, GLFW_KEY_R) == GLFW_PRESS)
	{
		textureShouldBeVisible = false;
	}
}

void Window::updateFrameTime()
{
	const float currentFrameTime{ static_cast<float>(glfwGetTime()) };
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
}

void Window::cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->cursorPosCallback(xposIn, yposIn);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->mouseButtonCallback(xpos, ypos);
}

void Window::scrollCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->scrollCallback(xposIn, yposIn);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->updateWindowSizeInfoForCamera(width, height);
	app->setFramebufferResized(true);
}

void Window::cursorPosCallback(double xposIn, double yposIn)
{
	const MouseButton mouseButton{ determineMouseButton() };
	if (mouseButton != MouseButton::None)
	{
		MouseModifierFlags modifiers;
		if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		{
			modifiers |= MouseModifierFlagBits::Alt;
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			modifiers |= MouseModifierFlagBits::Ctrl;
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			modifiers |= MouseModifierFlagBits::Shift;
		}
		cameraHandler.processMouseMovement(glm::ivec2(static_cast<int>(xposIn), static_cast<int>(yposIn)), mouseButton, modifiers);
	}
}

MouseButton Window::determineMouseButton() const
{
	if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		return MouseButton::Left;
	}
	if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		return MouseButton::Middle;
	}
	if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		return MouseButton::Right;
	}
	return MouseButton::None;
}

void Window::mouseButtonCallback(double xPosIn, double yPosIn)
{
	cameraHandler.setMousePosition(glm::ivec2(static_cast<int>(xPosIn), static_cast<int>(yPosIn)));
}

void Window::scrollCallback(double xoffset, double yoffset)
{
	cameraHandler.processMouseScroll(static_cast<int>(yoffset));
}

void Window::setFramebufferResized(bool framebufferResized)
{
	this->framebufferResized = framebufferResized;
}

void Window::updateWindowSizeInfoForCamera(int width, int height)
{
	cameraHandler.setWindowSize(glm::ivec2(width, height));
}