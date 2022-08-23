#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "WindowHandler.h"
#include "Camera.h"

class Window : public WindowHandler
{
public:
	Window(const int width, const int height, const std::string& title);
	~Window();
	
	GLFWwindow* getGLFWWindow() const noexcept;
	WindowSize getFramebufferSize() const override;
	void waitEvents() const override;
	void open(std::function<void(WindowHandler&, const Camera&)> drawFrame);
	static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	void mouseCallback(double xposIn, double yposIn);

private:
	void setGlfwWindowHints() const;
	void createGlfwWindow();
	void processKeyboard();
	void updateFrameTime();

	const int width;
	const int height;
	const std::string title;
	GLFWwindow* glfwWindow;
	Camera camera;
	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	bool firstMouse = true;
	float lastX, lastY;
};