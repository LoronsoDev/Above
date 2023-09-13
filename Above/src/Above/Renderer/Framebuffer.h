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
		virtual const FramebufferProperties& GetProperties() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<Framebuffer> Create(const FramebufferProperties& props);
	};
}