#include "Camera.h"

namespace MyWorld
{
	float Camera::view[16];
	float Camera::proj[16];

	void Camera::Init(CameraParam param)
	{
		const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -10.0f };
		bx::mtxLookAt(view, eye, at);
		bx::mtxProj(proj, 60.0f, float(param.windowSize.width) / float(param.windowSize.height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
	}

	void Camera::Terminate()
	{

	}

	void Camera::Begin()
	{
		bgfx::setViewTransform(0, view, proj);
	}

	void Camera::End()
	{

	}
}
