#include "Camera.h"

#define WALK_SPEED 16.0f
#define SPRINT_SPEED 25.0f
#define CRAWL_SPEED 10.0f

namespace MyWorld
{
	const glm::vec3 Camera::WORLD_UP = { 0.0f, 0.0f, 1.0f };

	glm::mat4 Camera::view;
	glm::mat4 Camera::proj;
	glm::vec3 Camera::eye = { 0.0f, 0.0f, 2.0f };
	glm::vec3 Camera::forward = { 1.0f, 0.0f, 0.0f };
	glm::vec3 Camera::up = { 0.0f, 0.0f, 1.0f };
	glm::vec3 Camera::right = glm::normalize(glm::cross(Camera::forward, Camera::up));

	bool Camera::moveUp = false;
	bool Camera::moveDown = false;
	bool Camera::moveLeft = false;
	bool Camera::moveRight = false;
	bool Camera::moveForward = false;
	bool Camera::moveBackward = false;

	void Camera::Init(CameraParam param)
	{
		view = glm::lookAt(eye, eye + forward, up);
		proj = glm::perspective(glm::pi<float>() / 3, float(param.windowSize.width) / float(param.windowSize.height), 1.0f, 100.0f);
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
		eye += glm::vec3{ forward.x * movement, forward.y * movement, 0.0f };
		view = glm::lookAt(eye, eye + forward, up);
	}

	void Camera::MoveBackward()
	{
		float movement = WALK_SPEED * (float)Time::getDeltaTime();
		eye += -glm::vec3{ forward.x * movement, forward.y * movement, 0.0f };
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
