#pragma once
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Core/Util.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace MyWorld
{
	struct WindowVersion
	{
		int major, minor;
		WindowVersion() : major(3), minor(3) {}
		WindowVersion(int _major, int _minor) : major(_major), minor(_minor) {}
	};

	struct WindowSize
	{
		int width, height;
		WindowSize() : width(960), height(540) {}
		WindowSize(int _w, int _h) : width(_w), height(_h) {}
	};

	struct WindowParams
	{
		WindowSize size;
		WindowVersion version;
		std::string title;
		WindowParams() : title("Untitled") {}
		WindowParams(WindowSize& _size, WindowVersion& _version, std::string& _title) : size(_size), version(_version), title(_title) {}
	};

	struct WindowData
	{
		std::function<void(Event& e)> eventHandler = nullptr;
		WindowData() {};
	};

	class Window
	{
	private:
		static GLFWwindow* m_window;
		static WindowData m_window_data;
		static WindowSize size;
		static void glfw_error_callback(int error, const char* description);
	public:
		static GLFWwindow* getGlfwWindowObj();
		static HWND getWindowHWND();
		static bool shouldWindowTerminate();
		static void Init(WindowParams &params);
		static void Terminate();
		static void Begin();
		static void End();
		static const WindowSize getWindowSize();
		static void setEventCallback(const std::function<void(Event& e)>& fn);
	};
}
