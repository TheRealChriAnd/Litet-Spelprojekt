#include <EnginePch.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Textures/Texture2D.h>

Framebuffer::Framebuffer(const FramebufferDesc& desc)
	: m_ppColor(),
	m_pDepth(nullptr),
	m_NumColorAttachments(0),
	m_Framebuffer(0),
	m_IsOwner(true),
	m_Width(0),
	m_Height(0),
	m_Samples(0)
{
	Create(desc);
}

Framebuffer::Framebuffer(const Texture* texture)
	: m_ppColor(),
	m_pDepth(nullptr),
	m_NumColorAttachments(0),
	m_Framebuffer(0),
	m_IsOwner(false),
	m_Width(0),
	m_Height(0),
	m_Samples(0)
{
	Create(texture);
}

Framebuffer::~Framebuffer()
{
	if (m_IsOwner)
	{
		for (uint32 i = 0; i < m_NumColorAttachments; i++)
		{
			if (m_ppColor[i] != nullptr)
			{
				delete m_ppColor[i];
				m_ppColor[i] = nullptr;
			}
		}

		if (m_pDepth != nullptr)
		{
			delete m_pDepth;
			m_pDepth = nullptr;
		}
	}

	if (glIsFramebuffer(m_Framebuffer))
	{
		glDeleteFramebuffers(1, &m_Framebuffer);
		m_Framebuffer = 0;
	}
}

void Framebuffer::Create(const FramebufferDesc& desc)
{
	TextureDesc textureDesc = {};
	textureDesc.GenerateMips = false;
	textureDesc.Width = desc.Width;
	textureDesc.Height = desc.Height;
	textureDesc.Samples = desc.Samples;

	m_NumColorAttachments = desc.NumColorAttachments;
	for (uint32 i = 0; i < desc.NumColorAttachments; i++)
	{
		if (desc.ColorAttchmentFormats[i] != TEX_FORMAT_UNKNOWN)
		{
			textureDesc.Format = desc.ColorAttchmentFormats[i];
			m_ppColor[i] = new Texture2D(nullptr, textureDesc, desc.SamplingParams);
		}
	}

	if (desc.DepthStencilFormat != TEX_FORMAT_UNKNOWN)
	{
		textureDesc.Format = desc.DepthStencilFormat;
		m_pDepth = new Texture2D(nullptr, textureDesc, desc.SamplingParams);
	}

	m_Width = desc.Width;
	m_Height = desc.Height;
	m_Samples = desc.Samples;

	CreateFramebuffer();
}

void Framebuffer::Create(const Texture* texture)
{
	//TODO: Fix this
	CreateFramebuffer();
}

void Framebuffer::CreateFramebuffer()
{
	glGenFramebuffers(1, &m_Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

	uint32 buf = 0;
	uint32 drawBuffers[8];
	for (uint32 i = 0; i < m_NumColorAttachments; i++)
	{
		if (m_ppColor[i] != nullptr)
		{
			drawBuffers[buf] = GL_COLOR_ATTACHMENT0 + buf;
			glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers[buf], m_ppColor[i]->GetType(), m_ppColor[i]->m_Texture, 0);

			buf++;
		}
	}

	glDrawBuffers(buf, drawBuffers);

	if (m_pDepth != nullptr)
	{
		if (m_pDepth->GetFormat() == TEX_FORMAT_DEPTH)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_pDepth->GetType(), m_pDepth->m_Texture, 0);
		}
		else if (m_pDepth->GetFormat() == TEX_FORMAT_DEPTH_STENCIL)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_pDepth->GetType(), m_pDepth->m_Texture, 0);
		}
		else
		{
			std::cout << "Not a valid format for depth" << std::endl;
		}
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error: Could not create framebuffer. Status: " << std::to_string(status) << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
