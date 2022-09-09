// Copyright(c) 2019, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

//Inspired by the implementation available here: https://github.com/KhronosGroup/Vulkan-Hpp/blob/master/samples/RayTracing

#pragma once

#if defined( _MSC_VER )
#  pragma warning( push )
#  pragma warning( disable : 4127 )  // conditional expression is constant (glm)
#elif defined( __clang__ )
// no need to ignore any warnings with clang
#elif defined( __GNUC__ )
#  if ( 10 <= __GNUC__ ) && ( 201703L < __cplusplus )
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wvolatile"
#  endif
#endif

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#if defined( _MSC_VER )
#  pragma warning( pop )
#elif defined( __clang__ )
// no need to ignore any warnings with clang
#elif defined( __GNUC__ )
#  if ( 10 <= __GNUC__ ) && ( 201703L < __cplusplus )
#    pragma GCC diagnostic pop
#  endif
#endif

#include "CameraHandler.h"

class Camera : public CameraHandler
{
public:

	enum class Mode
	{
		Examine,
		Fly,
		Walk,
		Trackball
	};
	
	Camera();

	const glm::vec3& getCameraPosition() const;
	const glm::vec3& getCenterPosition() const;
	Mode  getMode() const;
	const glm::ivec2& getMousePosition() const;
	float getRoll() const;
	float getSpeed() const;
	const glm::vec3& getUpVector() const;
	const glm::u32vec2& getWindowSize() const;
	const glm::mat4& getViewMatrix() const override;
	float getZoom() const override;
	void processKeyboard(CameraMovement direction, float deltaTime) override;
	Action processMouseMovement(const glm::ivec2& position, MouseButton mouseButton, MouseModifierFlags& modifiers) override;
	void processMouseScroll(int value) override;
	void setLookat(const glm::vec3& cameraPosition, const glm::vec3& centerPosition, const glm::vec3& upVector) override;
	void setMode(Mode mode);
	void setMousePosition(const glm::ivec2& position) override;
	void setRoll(float roll);  // roll in radians
	void setSpeed(float speed);
	void setWindowSize(const glm::ivec2& size) override;
	
	
private:
	void   dolly(const glm::vec2& delta);
	void   motion(const glm::ivec2& position, Action action = Action::None);
	void   orbit(const glm::vec2& delta, bool invert = false);
	void   pan(const glm::vec2& delta);
	double projectOntoTBSphere(const glm::vec2& p);
	void   trackball(glm::ivec2 const& position);
	void   update();

	glm::vec3 cameraPosition = glm::vec3(10, 10, 10);
	glm::vec3 centerPosition = glm::vec3(0, 0, 0);
	glm::vec3 upVector = glm::vec3(0, 1, 0);
	float roll = 0;  // Rotation around the Z axis in RAD
	glm::mat4 matrix = glm::mat4(1);
	glm::u32vec2 windowSize = glm::u32vec2(1, 1);
	float speed = 30.0f;
	float movementSpeed = 2.5f;
	float zoom = 45.0f;
	glm::ivec2 mousePosition = glm::ivec2(0, 0);
	Mode mode = Mode::Examine;
	const float trackballSize = 0.8f;
};
