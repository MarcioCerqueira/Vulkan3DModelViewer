#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "WindowHandler.h"
#include "CameraHandler.h"
#include "structs/MouseInfo.h"

class Window : public WindowHandler
{
public:
	Window(const WindowSize& windowSize, const std::string& title, CameraHandler& cameraHandler);
	~Window();
	
	GLFWwindow* getGLFWWindow() const;
	WindowSize getFramebufferSize() const override;
	void waitEvents() const override;
	void open(std::function<void(WindowHandler&, CameraHandler&)> drawFrame);
	
	static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void mouseCallback(double xposIn, double yposIn);
	void scrollCallback(double xoffset, double yoffset);

private:
	void setGlfwWindowHints() const;
	void createGlfwWindow();
	void setCallbacks();
	void initializeMouseInfo();
	void processKeyboard();
	void updateFrameTime();
	void updateMouseInfo(float lastX, float lastY);

	const int width;
	const int height;
	const std::string title;
	GLFWwindow* glfwWindow;
	CameraHandler& cameraHandler;
	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	MouseInfo mouseInfo;
};