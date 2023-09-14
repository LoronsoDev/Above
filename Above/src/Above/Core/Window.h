#pragma once

#include "abpch.h"

#include "Above/Core/Core.h"
#include "Above/Events/Event.h"

namespace Above
{
	/// <summary>
	/// Window general properties.
	/// </summary>
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "ABOVE",
					uint32_t width = 1280,
					uint32_t height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	// Intereface representing a desktop system based window.
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		
		inline virtual void* GetNativeWindow() const = 0;

		/// <summary>
		/// Creates a window.
		/// </summary>
		/// <param name="props">Properties of the window. Sets default values if not specified.</param>
		/// <returns>Window pointer</returns>
		static Window* Create(const WindowProps& props = WindowProps());
	};
}
