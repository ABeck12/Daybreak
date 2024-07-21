#pragma once

#include <string>
#include <stdint.h>

#include "Daybreak/Events/Event.h"

namespace Daybreak
{
	struct WindowSpecifications
	{
		std::string Title = "Daybreak Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		std::string IconFilepath = "../Resources/DaybreakLogo.png";
		bool Maximized = false;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetMaximized(bool maximized) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowSpecifications& props = WindowSpecifications());
	};
}