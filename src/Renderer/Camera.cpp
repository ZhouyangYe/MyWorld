#include "Camera.h"

#define WALK_SPEED 2.0f
#define SPRINT_SPEED 3.0f
#define CRAWL_SPEED 1.0f

namespace MyWorld
{
	const glm::vec3 Camera::WORLD_UP = { 0.0f, 0.0f, 1.0f };

	glm::mat4 Camera::view;
	glm::mat4 Camera::proj;
	glm::vec3 Camera::eye = { -6.0f, 0.0f, 200.0f };
	glm::vec3 Camera::forward = { 1.0f, 0.0f, 0.0f };
	glm::vec3 Camera::up = { 0.0f, 0.0f, 1.0f };
	glm::vec3 Camera::right = glm::normalize(glm::cross(Camera::forward, Camera::up));

	bool Camera::moveUp = false;
	bool Camera::moveDown = false;
	bool Camera::moveLeft = false;
	bool Camera::moveRight = false;
	bool Camera::moveForward = false;
	bool Camera::moveBackward = false;

	const glm::vec3& Camera::getCameraCoords()
	{
		return eye;
	}

	void Camera::Init(CameraParam param)
	{
		view = glm::lookAt(eye, eye + forward, up);
		proj = glm::perspective(glm::radians(75.0f), float(param.windowSize.width) / float(param.windowSize.height), 0.01f, 128.0f);
	}

	void Camera::Terminate()
	{

	}

	void Camera::Begin()
	{
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
		bgfx::setViewTransform(0, &view, &proj);
	}

	void Camera::End()
	{

	}

	void Camera::MoveUp()
	{
		eye.z += WALK_SPEED * Time::getDeltaTime();
   		view = glm::lookAt(eye, eye + forward, up);
	}

	void Camera::MoveDown()
	{
		eye.z += -WALK_SPEED * Time::getDeltaTime();
		view = glm::lookAt(eye, eye + forward, up);
	}

	void Camera::MoveLeft()
	{
		eye += -right * WALK_SPEED * (float)Time::getDeltaTime();
		view = glm::lookAt(eye, eye + forward, up);
	}

	void Camera::MoveRight()
	{
		eye += right * WALK_SPEED * (float)Time::getDeltaTime();
		view = glm::lookAt(eye, eye + forward, up);
	}

	void Camera::MoveForward()
	{
		float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		view = glm::lookAt(eye, eye + forward, up);
	}

	void Camera::MoveBackward()
	{
		float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += -glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		view = glm::lookAt(eye, eye + forward, up);
	}

	void Camera::Rotate(glm::vec2 delta)
	{
		forward = glm::mat3(
			glm::rotate(-delta.x * (float)Time::getDeltaTime(), WORLD_UP) *
			glm::rotate(-delta.y * (float)Time::getDeltaTime(), right)
		) * forward;

		right = glm::mat3(
			glm::rotate(-delta.x * (float)Time::getDeltaTime(), WORLD_UP)
		) * right;

		up = glm::normalize(glm::cross(right, forward));

		view = glm::lookAt(eye, eye + forward, up);
	}
}
