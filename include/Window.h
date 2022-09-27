#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "WindowHandler.h"
#include "CameraHandler.h"

class Window : public WindowHandler
{
public:
	Window(const WindowSize& windowSize, const std::string& title, CameraHandler& cameraHandler);
	~Window();
	
	GLFWwindow* getGLFWWindow() const;
	WindowSize getFramebufferSize() const override;
	void waitEvents() const override;
	bool showTexture() const override;
	void open(std::function<void(WindowHandler&, CameraHandler&, bool)> drawFrame);
	
	static void cursorPosCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	void cursorPosCallback(double xposIn, double yposIn);
	MouseButton determineMouseButton() const;
	void mouseButtonCallback(double xPosIn, double yPosIn);
	void scrollCallback(double xoffset, double yoffset);
	void setFramebufferResized(bool framebufferResized);
	void updateWindowSizeInfoForCamera(int width, int height);

private:
	void setGlfwWindowHint() const;
	void createGlfwWindow();
	void setCallbacks();
	void initializeCameraHandler();
	void processKeyboard();
	void updateFrameTime();

	const int width;
	const int height;
	const std::string title;
	GLFWwindow* glfwWindow;
	CameraHandler& cameraHandler;
	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	bool framebufferResized = false;
	bool textureShouldBeVisible = true;
};