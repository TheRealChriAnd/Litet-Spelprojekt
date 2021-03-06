#pragma once
#include "Texture.h"

class Texture2D;

//Maximum amount of rendertargets usally is 8
struct FramebufferDesc
{
	TEX_FORMAT DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	TEX_FORMAT ColorAttchmentFormats[8];
	TextureParams SamplingParams = TextureParams();
	uint32 NumColorAttachments = 0;
	uint32 Width = 0;
	uint32 Height = 0;
	uint32 Samples = 1;
};

class API Framebuffer
{
	friend class GLContext;

public:
	Framebuffer(Framebuffer&& other) = delete;
	Framebuffer(const Framebuffer& other) = delete;
	Framebuffer& operator=(Framebuffer&& other) = delete;
	Framebuffer& operator=(const Framebuffer& other) = delete;

	Framebuffer(const FramebufferDesc& desc);
	Framebuffer(Texture2D** ppColor, uint32 numTextures, Texture2D* pDepthStencil);
	~Framebuffer();

	Texture* GetColorAttachment(uint32 index) const;
	Texture* GetDepthAttachment() const;
	uint32 GetColorAttachmentCount() const;
	uint32 GetWidth() const;
	uint32 GetHeight() const;

private:
	void Create(const FramebufferDesc& desc);
	void Create(Texture2D** ppColor, uint32 numTextures, Texture2D* pDepthStencil);
	void CreateFramebuffer();

private:
	Texture* m_ppColor[8];
	Texture* m_pDepthStencil;
	uint32 m_NumColorAttachments;
	uint32 m_Framebuffer;
	uint32 m_Width;
	uint32 m_Height;
	uint32 m_Samples;
	bool m_IsOwner;
};

inline Texture* Framebuffer::GetColorAttachment(uint32 index) const
{
	assert(index < m_NumColorAttachments);
	return m_ppColor[index];
}

inline Texture* Framebuffer::GetDepthAttachment() const
{
	return m_pDepthStencil;
}

inline uint32 Framebuffer::GetColorAttachmentCount() const
{
	return m_NumColorAttachments;
}

inline uint32 Framebuffer::GetWidth() const
{
	return m_Width;
}

inline uint32 Framebuffer::GetHeight() const
{
	return m_Height;
}
