#pragma once
#include "Renderer.h"
#include "Core/Time.h"

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
		static glm::vec3 forward;
		static glm::vec3 up;
		static glm::vec3 right;
	public:
		struct CameraParam
		{
			WindowSize windowSize;
		};
		static const glm::vec3 WORLD_UP;
		static bool moveUp;
		static bool moveDown;
		static bool moveLeft;
		static bool moveRight;
		static bool moveForward;
		static bool moveBackward;
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
	};
}
