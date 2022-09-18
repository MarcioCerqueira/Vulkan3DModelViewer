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

// ignore warning 4127: conditional expression is constant
#if defined( _MSC_VER )
#  pragma warning( disable : 4127 )
#elif defined( __clang__ )
#  if ( 10 <= __clang_major__ )
#    pragma clang diagnostic ignored "-Wdeprecated-volatile"  // to keep glm/detail/type_half.inl compiling
#  endif
#elif defined( __GNUC__ )
// don't know how to switch off that warning here
#else
// unknow compiler... just ignore the warnings for yourselves ;)
#endif

#include "Camera.h"

template <typename T>
bool isZero(const T& _a)
{
	return fabs(_a) < std::numeric_limits<T>::epsilon();
}

template <typename T>
bool isOne(const T& _a)
{
	return areEqual(_a, (T)1);
}

inline float sign(float s)
{
	return (s < 0.f) ? -1.f : 1.f;
}

Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& centerPosition, const glm::vec3& upVector) : cameraPosition(cameraPosition), centerPosition(centerPosition), upVector(upVector)
{
	update();
}

void Camera::update()
{
	matrix = glm::lookAt(cameraPosition, centerPosition, upVector);
	if (!isZero(roll))
	{
		matrix = matrix * glm::rotate(roll, glm::vec3(0, 0, 1));
	}
}

const glm::vec3& Camera::getCameraPosition() const
{
	return cameraPosition;
}

const glm::vec3& Camera::getCenterPosition() const
{
	return centerPosition;
}

Camera::Mode Camera::getMode() const
{
	return mode;
}

const glm::ivec2& Camera::getMousePosition() const
{
	return mousePosition;
}

float Camera::getRoll() const
{
	return roll;
}

float Camera::getSpeed() const
{
	return speed;
}

const glm::vec3& Camera::getUpVector() const
{
	return upVector;
}

const glm::u32vec2& Camera::getWindowSize() const
{
	return windowSize;
}

Action Camera::processMouseMovement(const glm::ivec2& position, MouseButton mouseButton, MouseModifierFlags& modifiers)
{
	Action curAction{ Action::None };
	switch (mouseButton)
	{
	case MouseButton::Left:
		if (((modifiers & MouseModifierFlagBits::Ctrl) && (modifiers & MouseModifierFlagBits::Shift)) || (modifiers & MouseModifierFlagBits::Alt))
		{
			curAction = mode == Mode::Examine ? Action::LookAround : Action::Orbit;
		}
		else if (modifiers & MouseModifierFlagBits::Shift)
		{
			curAction = Action::Dolly;
		}
		else if (modifiers & MouseModifierFlagBits::Ctrl)
		{
			curAction = Action::Pan;
		}
		else
		{
			curAction = mode == Mode::Examine ? Action::Orbit : Action::LookAround;
		}
		break;
	case MouseButton::Middle: curAction = Action::Pan; break;
	case MouseButton::Right: curAction = Action::Dolly; break;
	default: assert(false);
	}
	assert(curAction != Action::None);
	motion(position, curAction);

	return curAction;
}

void Camera::motion(const glm::ivec2& position, Action action)
{
	glm::vec2 delta(float(position[0] - mousePosition[0]) / float(windowSize[0]),
		float(position[1] - mousePosition[1]) / float(windowSize[1]));

	switch (action)
	{
	case Action::Orbit:
		if (mode == Mode::Trackball)
		{
			orbit(delta, true);  // trackball(position);
		}
		else
		{
			orbit(delta, false);
		}
		break;
	case Action::Dolly: dolly(delta); break;
	case Action::Pan: pan(delta); break;
	case Action::LookAround:
		if (mode == Mode::Trackball)
		{
			trackball(position);
		}
		else
		{
			orbit(glm::vec2(delta[0], -delta[1]), true);
		}
		break;
	default: break;
	}

	update();

	mousePosition = position;
}

void Camera::setMode(Mode mode)
{
	this->mode = mode;
}

void Camera::setMousePosition(const glm::ivec2& position)
{
	this->mousePosition = position;
}

void Camera::setRoll(float roll)
{
	this->roll = roll;
	update();
}

void Camera::setSpeed(float speed)
{
	this->speed = speed;
}

void Camera::setWindowSize(glm::ivec2 const& size)
{
	this->windowSize = size;
}

void Camera::processMouseScroll(int value)
{
	float fValue{ static_cast<float>(value) };
	float dx{ (fValue * std::abs(fValue)) / static_cast<float>(windowSize[0]) };

	glm::vec3 z{ cameraPosition - centerPosition };
	float length{ z.length() * 0.1f };
	length = length < 0.001f ? 0.001f : length;

	dx *= speed;
	dolly(glm::vec2(dx, dx));
	update();
}

void Camera::dolly(const glm::vec2& delta)
{
	glm::vec3 z{ centerPosition - cameraPosition };
	float length{ glm::length(z) };

	// We are at the point of interest, and don't know any direction, so do nothing!
	if (isZero(length))
	{
		return;
	}

	// Use the larger movement.
	float dd;
	if (mode != Mode::Examine)
	{
		dd = -delta[1];
	}
	else
	{
		dd = fabs(delta[0]) > fabs(delta[1]) ? delta[0] : -delta[1];
	}

	float factor{ speed * dd / length };

	// Adjust speed based on distance.
	length /= 10;
	length = length < 0.001f ? 0.001f : length;
	factor *= length;

	// Don't move to or through the point of interest.
	if (1.0f <= factor)
	{
		return;
	}

	z *= factor;

	// Not going up
	if (mode == Mode::Walk)
	{
		if (upVector.y > upVector.z)
		{
			z.y = 0;
		}
		else
		{
			z.z = 0;
		}
	}

	cameraPosition += z;

	// In fly mode, the interest moves with us.
	if (mode != Mode::Examine)
	{
		centerPosition += z;
	}
}

void Camera::orbit(const glm::vec2& delta, bool invert)
{
	if (isZero(delta[0]) && isZero(delta[1]))
	{
		return;
	}

	// Full width will do a full turn
	const float dx{ delta[0] * float(glm::two_pi<float>()) };
	const float dy{ delta[1] * float(glm::two_pi<float>()) };

	// Get the camera
	const glm::vec3 origin(invert ? cameraPosition : centerPosition);
	const glm::vec3 position(invert ? centerPosition : cameraPosition);

	// Get the length of sight
	glm::vec3 centerToEye(position - origin);
	const float radius{ glm::length(centerToEye) };
	centerToEye = glm::normalize(centerToEye);

	// Find the rotation around the UP axis (Y)
	const glm::vec3 zAxis(centerToEye);
	const glm::mat4 yRotation{ glm::rotate(-dx, upVector) };

	// Apply the (Y) rotation to the eye-center vector
	glm::vec4 tmpVector{ yRotation * glm::vec4(centerToEye.x, centerToEye.y, centerToEye.z, 0.0f) };
	centerToEye = glm::vec3(tmpVector.x, tmpVector.y, tmpVector.z);

	// Find the rotation around the X vector: cross between eye-center and up (X)
	glm::vec3 xAxis{ glm::cross(upVector, zAxis) };
	xAxis = glm::normalize(xAxis);
	const glm::mat4 xRotation{ glm::rotate(-dy, xAxis) };

	// Apply the (X) rotation to the eye-center vector
	tmpVector = xRotation * glm::vec4(centerToEye.x, centerToEye.y, centerToEye.z, 0);
	const glm::vec3 rotatedVector(tmpVector.x, tmpVector.y, tmpVector.z);
	if (sign(rotatedVector.x) == sign(centerToEye.x))
	{
		centerToEye = rotatedVector;
	}

	// Make the vector as long as it was originally
	centerToEye *= radius;

	// Finding the new position
	const glm::vec3 newPosition{ centerToEye + origin };

	if (!invert)
	{
		cameraPosition = newPosition;  // Normal: change the position of the camera
	}
	else
	{
		centerPosition = newPosition;  // Inverted: change the interest point
	}
}

void Camera::pan(const glm::vec2& delta)
{
	glm::vec3 z(cameraPosition - centerPosition);
	const float length{ static_cast<float>(glm::length(z)) / 0.785f };  // 45 degrees
	z = glm::normalize(z);
	glm::vec3 x{ glm::normalize(glm::cross(upVector, z)) };
	glm::vec3 y{ glm::normalize(glm::cross(z, x)) };
	x *= -delta[0] * length;
	y *= delta[1] * length;

	if (mode == Mode::Fly)
	{
		x = -x;
		y = -y;
	}

	cameraPosition += x + y;
	centerPosition += x + y;
}

double Camera::projectOntoTBSphere(const glm::vec2& p)
{
	const double d{ length(p) };
	if (d < trackballSize * 0.70710678118654752440)
	{
		// inside sphere
		return sqrt(trackballSize * trackballSize - d * d);
	}
	else
	{
		// on hyperbola
		const double t{ trackballSize / 1.41421356237309504880 };
		return t * t / d;
	}
}

void Camera::trackball(glm::ivec2 const& position)
{
	const glm::vec2 p0(2 * (mousePosition[0] - windowSize[0] / 2) / double(windowSize[0]),
		2 * (windowSize[1] / 2 - mousePosition[1]) / double(windowSize[1]));
	const glm::vec2 p1(2 * (position[0] - windowSize[0] / 2) / double(windowSize[0]),
		2 * (windowSize[1] / 2 - position[1]) / double(windowSize[1]));
	// determine the z coordinate on the sphere
	const glm::vec3 pTB0(p0[0], p0[1], projectOntoTBSphere(p0));
	const glm::vec3 pTB1(p1[0], p1[1], projectOntoTBSphere(p1));
	// calculate the rotation axis via cross product between p0 and p1
	const glm::vec3 axis{ glm::normalize(glm::cross(pTB0, pTB1)) };
	// calculate the angle
	const float t{ std::clamp(glm::length(pTB0 - pTB1) / (2.f * trackballSize), -1.0f, 1.0f) };
	const float rad{ 2.0f * asin(t) };
	const glm::vec4 rot_axis{ matrix * glm::vec4(axis, 0) };
	const glm::mat4 rot_mat{ glm::rotate(rad, glm::vec3(rot_axis.x, rot_axis.y, rot_axis.z)) };
	const glm::vec3 pnt{ cameraPosition - centerPosition };
	const glm::vec4 pnt2{ rot_mat * glm::vec4(pnt.x, pnt.y, pnt.z, 1) };
	const glm::vec4 up2{ rot_mat * glm::vec4(upVector.x, upVector.y, upVector.z, 0) };
	cameraPosition = centerPosition + glm::vec3(pnt2.x, pnt2.y, pnt2.z);
	upVector = glm::vec3(up2.x, up2.y, up2.z);
}

const glm::mat4& Camera::getViewMatrix() const
{
	return matrix;
}

float Camera::getZoom() const
{
	return zoom;
}

void Camera::printCameraData() const
{
	std::cout << std::endl;
	std::cout << "=========== CAMERA DATA ===========" << std::endl;
	std::cout << "Camera Position: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
	std::cout << "Target Position: " << centerPosition.x << " " << centerPosition.y << " " << centerPosition.z << std::endl;
	std::cout << "Up Vector: " << upVector.x << " " << upVector.y << " " << upVector.z << std::endl;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	const float velocity{ movementSpeed * deltaTime };
	const glm::vec3 front{ glm::normalize(centerPosition - cameraPosition) };
	const glm::vec3 right{ glm::normalize(glm::cross(front, upVector)) };
	upVector = glm::normalize(glm::cross(right, front));
	switch (direction)
	{
	case CameraMovement::Forward:
		cameraPosition += front * velocity;
		break;
	case CameraMovement::Backward:
		cameraPosition -= front * velocity;
		break;
	case CameraMovement::Left:
		cameraPosition -= right * velocity;
		break;
	case CameraMovement::Right:
		cameraPosition += right * velocity;
		break;
	}
	update();
}