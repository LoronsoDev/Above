#pragma once

#include <Above/Core/Core.h>

namespace Above
{
	struct FramebufferProperties
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapchainTarget = false;
	};

	class Framebuffer
	{
	public:

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual const FramebufferProperties& GetProperties() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		static Ref<Framebuffer> Create(const FramebufferProperties& props);
	};
}