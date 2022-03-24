#pragma once
#include "Renderer.h"
#include "Core/Time.h"
#include "Tools.h"

namespace MyWorld
{
	class Camera
	{
	private:
		struct WindowSize
		{
			int width, height;
		};
		static glm::mat4 view;
		static glm::mat4 proj;
		static glm::vec3 eye;
		static glm::vec3 cam;
		static float zoom;
		static glm::vec3 offset;
		static glm::vec3 forward;
		static glm::vec3 up;
		static glm::vec3 right;
		static bool isCameraMoved;
		static bool isCameraRotated;
		static void updateCam();
	public:
		struct CameraParam
		{
			WindowSize windowSize;
			float view_distance;
			glm::vec3 spawnLocation;
			glm::vec3 offset{ 0.0f, 0.0f, 0.0f };
			glm::vec3 forward{ 1.0f, 0.0f, 0.0f };
			glm::vec3 up{ 0.0f, 0.0f, 1.0f };
		};
		static const glm::vec3 WORLD_UP;
		static bool cameraMoveDisabled;
		static bool freeCamera;
		static bool moveUp;
		static bool moveDown;
		static bool moveLeft;
		static bool moveRight;
		static bool moveForward;
		static bool moveBackward;
		static const glm::vec3& getEyeCoords();
		static const glm::vec3& getCameraCoords();
		static const glm::vec3& getForwardVec();
		static void Init(CameraParam param);
		static void Terminate();
		static void Begin();
		static void End();
		static void MoveUp();
		static void MoveDown();
		static void MoveLeft();
		static void MoveRight();
		static void MoveForward();
		static void MoveBackward();
		static void Rotate(glm::vec2 delta);
		static const bool& cameraMoved();
		static const bool& cameraRotated();
		static void zoomIn();
		static void zoomOut();
		static void setCamPos(glm::vec3& pos);
	};
}
