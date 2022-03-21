#include "Camera.h"

#define WALK_SPEED 2.0f
#define SPRINT_SPEED 3.0f
#define CRAWL_SPEED 1.0f
#define MAX_DISTANCE 5.0f

namespace MyWorld
{
	const glm::vec3 Camera::WORLD_UP = { 0.0f, 0.0f, 1.0f };

	glm::mat4 Camera::view;
	glm::mat4 Camera::proj;
	glm::vec3 Camera::eye;
	float Camera::zoom;
	glm::vec3 Camera::offset;
	glm::vec3 Camera::forward = { 1.0f, 0.0f, 0.0f };
	glm::vec3 Camera::up = { 0.0f, 0.0f, 1.0f };
	glm::vec3 Camera::right = glm::normalize(glm::cross(Camera::forward, Camera::up));

	bool Camera::moveUp = false;
	bool Camera::moveDown = false;
	bool Camera::moveLeft = false;
	bool Camera::moveRight = false;
	bool Camera::moveForward = false;
	bool Camera::moveBackward = false;

	bool Camera::isCameraMoved = false;
	bool Camera::isCameraRotated = false;

	void Camera::zoomIn()
	{
		if (zoom == 0.0f) return;
		zoom -= 0.3;
		if (zoom < 0) zoom = 0;
		offset = zoom * forward;
		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::zoomOut()
	{
		if (zoom == MAX_DISTANCE) return;
		zoom += 0.3;
		if (zoom > MAX_DISTANCE) zoom = MAX_DISTANCE;
		offset = zoom * forward;
		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::setCamPos(glm::vec3& pos)
	{
		eye = pos;
	}

	const glm::vec3& Camera::getCameraCoords()
	{
		return eye;
	}

	const glm::vec3& Camera::getForwardVec()
	{
		return forward;
	}

	const bool& Camera::cameraMoved()
	{
		return isCameraMoved;
	}

	const bool& Camera::cameraRotated()
	{
		return isCameraRotated;
	}

	void Camera::Init(CameraParam param)
	{
		eye = param.spawnLocation;
		view = glm::lookAt(eye, eye + forward, up);
		proj = glm::perspective(glm::radians(75.0f), float(param.windowSize.width) / float(param.windowSize.height), 0.01f, param.view_distance);
	}

	void Camera::Terminate()
	{}

	void Camera::Begin()
	{
		isCameraMoved = moveUp || moveDown || moveLeft || moveRight || moveForward || moveBackward;

		if (moveUp)
			MoveUp();
		if (moveDown)
			MoveDown();
		if (moveLeft)
			MoveLeft();
		if (moveRight)
			MoveRight();
		if (moveForward)
			MoveForward();
		if (moveBackward)
			MoveBackward();

		bgfx::setViewTransform(Tools::DEFAULT_VIEW_ID, &view, &proj);
	}

	void Camera::End()
	{
		isCameraMoved = false;
		isCameraRotated = false;
	}

	void Camera::MoveUp()
	{
		eye.z += WALK_SPEED * Time::getDeltaTime();
   		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::MoveDown()
	{
		eye.z += -WALK_SPEED * Time::getDeltaTime();
		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::MoveLeft()
	{
		eye += -right * WALK_SPEED * (float)Time::getDeltaTime();
		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::MoveRight()
	{
		eye += right * WALK_SPEED * (float)Time::getDeltaTime();
		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::MoveForward()
	{
		float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::MoveBackward()
	{
		float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += -glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		view = glm::lookAt(eye - offset, eye + forward, up);
	}

	void Camera::Rotate(glm::vec2 delta)
	{
		isCameraRotated = true;
		forward = glm::mat3(
			glm::rotate(-delta.x * (float)Time::getDeltaTime(), WORLD_UP) *
			glm::rotate(-delta.y * (float)Time::getDeltaTime(), right)
		) * forward;

		right = glm::mat3(
			glm::rotate(-delta.x * (float)Time::getDeltaTime(), WORLD_UP)
		) * right;

		up = glm::normalize(glm::cross(right, forward));

		offset = zoom * forward;

		view = glm::lookAt(eye - offset, eye + forward, up);
	}
}
