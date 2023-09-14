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

		virtual void Resize(uint32_t width, uint32_t height) override;
		const FramebufferProperties& GetProperties() const override { return m_FramebufferProps; }
		uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferProperties m_FramebufferProps;
	};
}