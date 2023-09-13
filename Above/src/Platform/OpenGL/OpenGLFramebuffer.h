#pragma once

#include <Above/Renderer/FrameBuffer.h>

namespace Above
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferProperties& props);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		const FramebufferProperties& GetProperties() const override { return m_FramebufferProps; }
		uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FramebufferProperties m_FramebufferProps;
	};
}