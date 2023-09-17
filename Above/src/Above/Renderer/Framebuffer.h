#pragma once

#include <Above/Core/Core.h>

namespace Above
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		RGBA8,

		DEPTH24STENCIL8,

		//default
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureProperties
	{
		FramebufferTextureProperties() = default;
		FramebufferTextureProperties(FramebufferTextureFormat format) : TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentProperties
	{
		FramebufferAttachmentProperties() = default;
		FramebufferAttachmentProperties(std::initializer_list<FramebufferTextureProperties> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureProperties> Attachments;
	};

	struct FramebufferProperties
	{
		uint32_t Width, Height;
		FramebufferAttachmentProperties Attachments;
		uint32_t Samples = 1;

		bool SwapchainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual const FramebufferProperties& GetProperties() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererID(uint32_t index = 0) const = 0;
		static Ref<Framebuffer> Create(const FramebufferProperties& props);
	};
}