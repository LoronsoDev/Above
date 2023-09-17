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
		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { AB_CORE_ASSERT(index < m_ColorAttachments.size(), "Index not valid"); return m_ColorAttachments[index]; }
		uint32_t GetDepthAttachmentRendererID(uint32_t index = 0) const override { AB_CORE_ASSERT(m_DepthAttachment, "There is no depth attachment"); return m_DepthAttachment; }

		void Bind() override;
		void Unbind() override;

	private:
		uint32_t fbo = 0;
		FramebufferProperties m_FramebufferProps;

		std::vector<FramebufferTextureProperties> m_ColorAttachmentProps;
		FramebufferTextureProperties m_DepthAttachmentProps = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}