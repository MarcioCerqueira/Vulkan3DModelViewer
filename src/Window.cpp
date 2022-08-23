#include "Window.h"

Window::Window(const int width, const int height, const std::string& title) : width(width), height(height), title(title), lastX(width/2.0f), lastY(height/2.0f)
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
	glfwSetWindowUserPointer(glfwWindow, this);
	glfwSetCursorPosCallback(glfwWindow, mouseCallback);
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

void Window::waitEvents() const
{
	glfwWaitEvents();
}

void Window::open(std::function<void(WindowHandler&, const Camera&)> drawFrame)
{
	while(!glfwWindowShouldClose(glfwWindow)) 
	{
		glfwPollEvents();
		updateFrameTime();
		processKeyboard();
		drawFrame(*this, camera);
		
	}
}

void Window::processKeyboard()
{
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwWindow, true);
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
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

void Window::mouseCallback(double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}