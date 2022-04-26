#include "Camera.h"

#define WALK_SPEED 2.0f
#define SPRINT_SPEED 3.0f
#define CRAWL_SPEED 1.0f
#define MAX_DISTANCE 5.0f

namespace MyWorld
{
	const glm::vec3 Camera::WORLD_UP{ 0.0f, 0.0f, 1.0f };

	glm::mat4 Camera::view;
	glm::mat4 Camera::proj;
	glm::vec3 Camera::eye;
	glm::vec3 Camera::cam;
	float Camera::zoom;
	glm::vec3 Camera::offset;
	glm::vec3 Camera::forward;
	glm::vec3 Camera::up;
	glm::vec3 Camera::right;

	bool Camera::freeCamera = true;
	bool Camera::cameraMoveDisabled = false;

	bool Camera::moveUp = false;
	bool Camera::moveDown = false;
	bool Camera::moveLeft = false;
	bool Camera::moveRight = false;
	bool Camera::moveForward = false;
	bool Camera::moveBackward = false;

	bool Camera::isCameraMoved = false;
	bool Camera::isCameraRotated = false;

	void Camera::updateCam()
	{
		cam = eye - offset;
		view = glm::lookAt(cam, eye + forward, up);
	}

	void Camera::zoomIn()
	{
		if (zoom == 0.0f) return;
		zoom -= 0.3;
		if (zoom < 0) zoom = 0;
		offset = zoom * forward;
		updateCam();
	}

	void Camera::zoomOut()
	{
		if (zoom == MAX_DISTANCE) return;
		zoom += 0.3;
		if (zoom > MAX_DISTANCE) zoom = MAX_DISTANCE;
		offset = zoom * forward;
		updateCam();
	}

	void Camera::setCamPos(glm::vec3& pos)
	{
		eye = pos;
		updateCam();
	}

	const glm::vec3& Camera::getCameraCoords()
	{
		return cam;
	}

	const glm::vec3& Camera::getEyeCoords()
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
		cam = eye - offset;
		forward = param.forward;
		zoom = param.zoom;
		offset = param.zoom * forward;
		up = param.up;
		right = glm::normalize(glm::cross(Camera::forward, Camera::up));
		updateCam();
		proj = glm::perspective(glm::radians(75.0f), float(param.windowSize.width) / float(param.windowSize.height), 0.01f, param.view_distance);
	}

	void Camera::Terminate()
	{}

	void Camera::Begin()
	{
		if (cameraMoveDisabled) return;

		if (moveUp && freeCamera)
			MoveUp();
		if (moveDown && freeCamera)
			MoveDown();
		if (moveLeft)
			MoveLeft();
		if (moveRight)
			MoveRight();
		if (moveForward)
			MoveForward();
		if (moveBackward)
			MoveBackward();

		isCameraMoved = moveUp || moveDown || moveLeft || moveRight || moveForward || moveBackward;
	}

	void Camera::End()
	{
		isCameraMoved = false;
		isCameraRotated = false;

		bgfx::setViewTransform(Tools::DEFAULT_VIEW_ID, &view, &proj);
	}

	void Camera::MoveUp()
	{
		eye.z += WALK_SPEED * Time::getDeltaTime();
		updateCam();
	}

	void Camera::MoveDown()
	{
		eye.z += -WALK_SPEED * Time::getDeltaTime();
		updateCam();
	}

	void Camera::MoveLeft()
	{
		eye += -right * WALK_SPEED * (float)Time::getDeltaTime();
		updateCam();
	}

	void Camera::MoveRight()
	{
		eye += right * WALK_SPEED * (float)Time::getDeltaTime();
		updateCam();
	}

	void Camera::MoveForward()
	{
		float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		updateCam();
	}

	void Camera::MoveBackward()
	{
		float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += -glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		updateCam();
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

		offset = zoom * forward;

		updateCam();
		isCameraRotated = true;
	}
}
