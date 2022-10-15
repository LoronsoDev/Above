#pragma once

#include "abpch.h"

#include "Above/Core.h"
#include "Above/Events/Event.h"

namespace Above
{
	/// <summary>
	/// Window general properties.
	/// </summary>
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "ABOVE",
					unsigned width = 1280,
					unsigned height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	// Intereface representing a desktop system based window.
	class ABOVE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual void OnUpdate() = 0;

		virtual unsigned GetWidth() const = 0;
		virtual unsigned GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		/// <summary>
		/// Creates a window.
		/// </summary>
		/// <param name="props">Properties of the window. Sets default values if not specified.</param>
		/// <returns>Window pointer</returns>
		static Window* Create(const WindowProps& props = WindowProps());
	};
}
